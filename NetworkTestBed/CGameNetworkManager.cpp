#include "3ngine.Core.h"

#pragma region INetworkService Properties
CNetworkSession* CGameNetworkManager::CurrentSession()
{
	return this->m_pCurrentSession;
}
#pragma endregion

#pragma region Constructors and Finalizers
CGameNetworkManager::CGameNetworkManager(CGame* game, char* host, char* port, char* identifier)
: CGameComponent(game)
{
	this->m_pHost = host;
	this->m_pPort = port;
	this->m_pIdentifier = identifier;
	this->m_pCurrentSession = NULL;

	game->Get_Services()->AddService(this);
}

CGameNetworkManager::~CGameNetworkManager()
{
	if(this->m_pBackgroundSession != NULL)
		delete this->m_pBackgroundSession;

	if(this->m_pCurrentSession != NULL)
		delete this->m_pCurrentSession;
}
#pragma endregion

#pragma region Methods
void CGameNetworkManager::UpdateClients(Packet packet, CGameTime* gameTime)
{
	std::string key = packet.Data;
	std::map<std::string, float>::iterator it;
	bool found = false;

	for ( it = this->m_pMachines.begin() ; it != this->m_pMachines.end(); it++ )
	{
		if((*it).first.compare(key) == 0)
		{
			(*it).second = 0;
			found = true;
		}
		else if((*it).second > 60)
			this->m_pMachines.erase(it);
		else
			(*it).second += gameTime->Get_ElapsedTime();
	}

	// add the machine into the cache
	if(!found)
		this->m_pMachines[key] = 0;
}

void CGameNetworkManager::CreateGame(Packet packet, CGameTime* gameTime)
{
	D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)packet.Data;
	if(this->m_pGameSessions.find(session->Identifier) == this->m_pGameSessions.end())
	{
		//add the game to the cache
		this->m_pGameSessions[session->Identifier] = *session;
	}
}

void CGameNetworkManager::UpdateGame(Packet packet, CGameTime* gameTime)
{
	D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)packet.Data;
	if(this->m_pGameSessions.find(session->Identifier) != this->m_pGameSessions.end())
	{
		//update game
		this->m_pGameSessions[session->Identifier] = *session;
	}
}

void CGameNetworkManager::DestroyGame(Packet packet, CGameTime* gameTime)
{
	D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)packet.Data;
	std::map<std::string, D3DX_SESSION_DESCRIPTION>::iterator it;
	if((it = this->m_pGameSessions.find(session->Identifier)) != this->m_pGameSessions.end())
		this->m_pGameSessions.erase(it);
}

void CGameNetworkManager::JoinGame(Packet packet, CGameTime* gameTime)
{
	D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)packet.Data;
	std::map<std::string, D3DX_SESSION_DESCRIPTION>::iterator it;
	if((it = this->m_pGameSessions.find(session->Identifier)) != this->m_pGameSessions.end())
	{
		(*it).second.OpenSlots--;

		this->m_pBackgroundSession->SendPacket(&Packet(PacketCommand::UpdateGame, "\0", (char*)&(*it).second, sizeof(D3DX_SESSION_DESCRIPTION)));
	}
}
#pragma endregion

#pragma region INetworkService Methods
CNetworkSession* CGameNetworkManager::CreateSession(NetworkSessionType type, int maxPlayers)
{
	if(this->m_pCurrentSession == NULL)
	{
		D3DX_SESSION_DESCRIPTION gameDesc;
		gameDesc.Type = type;
		strcpy(gameDesc.Identifier, "@id/game/1");
		strcpy(gameDesc.Host,this->m_pHost);
		gameDesc.Port = 5467;
		gameDesc.MaxPlayers = maxPlayers;
		gameDesc.OpenSlots = maxPlayers - 1;

		// register the game
		this->m_pBackgroundSession->SendPacket(&Packet(PacketCommand::CreateGame, "\0", (char*)&gameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
		
		// send back the new sessions credentials
		char port[6];
		itoa(gameDesc.Port, port, 10);
		this->m_pCurrentSession = new CNetworkSession(this->Get_Game(), gameDesc.Host, port, gameDesc.Identifier); 
		this->Get_Game()->Get_Components()->Add(this->m_pCurrentSession);
		this->m_pCurrentSession->SetToHost();
		this->m_pCurrentSession->Connect();
	}

	return this->m_pCurrentSession;
}

std::map<std::string, D3DX_SESSION_DESCRIPTION> CGameNetworkManager::Find(NetworkSessionType)
{
	return this->m_pGameSessions;
}

std::map<std::string, float> CGameNetworkManager::NetworkedClients()
{
	return this->m_pMachines;
}

CNetworkSession* CGameNetworkManager::Join(D3DX_SESSION_DESCRIPTION description)
{
	std::map<std::string, D3DX_SESSION_DESCRIPTION>::iterator it;
	if((it = this->m_pGameSessions.find(description.Identifier)) != this->m_pGameSessions.end())
	{
		if((*it).second.OpenSlots > 0)
		{
			this->m_pBackgroundSession->SendPacket(&Packet(PacketCommand::JoinGame, "\0", (char*)&description, sizeof(D3DX_SESSION_DESCRIPTION)));

			char port[6];
			itoa(description.Port, port, 10);
			this->m_pCurrentSession = new CNetworkSession(this->Get_Game(), description.Host, port, this->m_pIdentifier); 
			this->Get_Game()->Get_Components()->Add(this->m_pCurrentSession);
			this->m_pCurrentSession->Connect();
		}
	}
	return NULL;
}
#pragma endregion

#pragma region Overriden Methods
void CGameNetworkManager::Initialize()
{
	CGameComponent::Initialize();

	this->m_pBackgroundSession = new CNetworkSession(this->Get_Game(), this->m_pHost, this->m_pPort, this->m_pIdentifier);
	this->Get_Game()->Get_Components()->Add(this->m_pBackgroundSession);
	this->m_pBackgroundSession->Set_KeepAlive(5);
	this->m_pBackgroundSession->Connect();
}

void CGameNetworkManager::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	if(this->m_pBackgroundSession == NULL && !this->m_pBackgroundSession->IsDataAvailable())
		return;

	int count = 0;
	Packet packet;
	while((packet = this->m_pBackgroundSession->GetNextPacket()).Command != PacketCommand::Empty && count < 10)
	{
		switch(packet.Command)
		{
			case PacketCommand::KeepAlive:
			{
				this->UpdateClients(packet, gameTime);
				break;
			}
			case PacketCommand::CreateGame:
			{
				this->CreateGame(packet, gameTime);
				break;
			}
			case PacketCommand::DestroyGame:
			{
				this->DestroyGame(packet, gameTime);
				break;
			}
			case PacketCommand::UpdateGame:
			{
				this->UpdateGame(packet, gameTime);
				break;
			}
			case PacketCommand::JoinGame:
			{
				this->JoinGame(packet, gameTime);
				break;
			}
		}

		count++;
	}
}
#pragma endregion