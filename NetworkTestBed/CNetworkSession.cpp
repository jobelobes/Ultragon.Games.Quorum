#include "3ngine.Core.h"

#pragma region Properties
NetworkSessionState CNetworkSession::Get_State()
{
	return this->m_eState;
}

CMulticastClient* CNetworkSession::Get_MulticastClient()
{
	return this->m_pClient;
}

float CNetworkSession::Get_KeepAlive()
{
	return this->m_fKeepAliveTimeout;
}

void CNetworkSession::Set_KeepAlive(float value)
{
	this->m_fKeepAliveTimeout = value;
}

bool CNetworkSession::Get_IsHost()
{
	return this->m_bIsHost;
}
#pragma endregion

#pragma region Constructors and Finalizers
CNetworkSession::CNetworkSession(CGame* game, char* host, char* port, char* identifier)
:CGameComponent(game)
{
	this->m_pHost = new char[strlen(host)];
	strcpy(this->m_pHost, host);

	this->m_pPort = new char[strlen(port)];
	strcpy(this->m_pPort, port);

	this->m_pIdentifier = new char[strlen(identifier)];
	strcpy(this->m_pIdentifier, identifier);

	this->m_bIsHost = false;
}

CNetworkSession::~CNetworkSession()
{
	if(this->m_pClient != NULL)
		delete this->m_pClient;

	if(this->m_pHost != NULL)
		delete [] this->m_pHost;
	if(this->m_pPort != NULL)
		delete [] this->m_pPort;
	if(this->m_pIdentifier != NULL)
		delete [] this->m_pIdentifier;
}
#pragma endregion 

#pragma region Methods
void CNetworkSession::SetToHost()
{
	this->m_bIsHost = true;
}

bool CNetworkSession::IsDataAvailable()
{
	Packet* packet = NULL;
	for(int i = 0; i < this->m_iReceiveBufferSize; i++)
	{
		if(this->m_iReceiveBufferUse[i] == 0)
			continue;

		packet = (Packet*)(this->m_pReceiveBuffer + i * sizeof(Packet));
		if(packet != NULL)
			return true;
	}
	return false;
}

bool CNetworkSession::IsDataAvailable(char* identifier)
{
	Packet* packet = NULL;
	for(int i = 0; i < this->m_iReceiveBufferSize; i++)
	{
		if(this->m_iReceiveBufferUse[i] == 0)
			continue;

		packet = (Packet*)(this->m_pReceiveBuffer + i * sizeof(Packet));
		if(strcmp(packet->Identifier, identifier) == 0 && packet != NULL)
			return true;
	}
	return false;
}

void CNetworkSession::ResetReady()
{
	if(this->m_eState != NetworkSessionState::Lobby)
		return;

	this->SendPacket(&Packet(PacketCommand::ResetReady, this->m_pIdentifier, "\0", 1));
}

void CNetworkSession::StartGame()
{
	if(this->m_eState != NetworkSessionState::Lobby)
		return;

	this->m_eState = NetworkSessionState::Playing;
}

void CNetworkSession::ClearPackets(char* identifier)
{
	Packet* packet = NULL;
	for(int i = 0; i < this->m_iReceiveBufferSize; i++)
	{
		if(this->m_iReceiveBufferUse[i] == 0)
			continue;

		packet = (Packet*)(this->m_pReceiveBuffer + i * sizeof(Packet));
		if(strcmp(packet->Identifier, identifier) == 0)
			this->m_iReceiveBufferUse[i] = 0;
	}
}

Packet CNetworkSession::GetNextPacket(char* identifier)
{
	int index = -1;
	Packet* packet = NULL;
	for(int i = 0; i < this->m_iReceiveBufferSize; i++)
	{
		if(this->m_iReceiveBufferUse[i] == 0)
			continue;

		packet = (Packet*)(this->m_pReceiveBuffer + i * sizeof(Packet));
		if(this->m_iReceiveBufferUse[i] > this->m_iReceiveBufferUse[index])
		{
			if(strcmp(packet->Identifier, identifier) == 0)
				index = i;
		}
	}
	if(index == -1 || index >= this->m_iReceiveBufferSize)
		return Packet(PacketCommand::Empty, "\0", "\0", 1);

	this->m_iReceiveBufferUse[index] = 0;
	return *(Packet*)(this->m_pReceiveBuffer + index * sizeof(Packet));
}

Packet CNetworkSession::GetNextPacket()
{
	return this->GetNextPacket("\0");
}

void CNetworkSession::SendKeepAlive()
{
	this->m_pKeepAliveTimer->Tick();
	if(this->m_pKeepAliveTimer->Get_TotalTime() > this->m_fKeepAliveTimeout)
	{
		Packet packet;
		packet.Command = PacketCommand::KeepAlive;
		packet.SetIdentifier("\0");
		packet.SetData(this->m_pIdentifier, strlen(this->m_pIdentifier) + 1);
		this->m_pClient->Send((char*)&packet, 0, sizeof(Packet));
		this->m_pKeepAliveTimer->Reset();

		printf("[%s]Sent Keepalive packet\n", this->m_pIdentifier);
	}
}

void CNetworkSession::SendPacket(Packet* packet)
{
	this->m_pClient->Send((char*)packet, 0, sizeof(Packet));
}

void CNetworkSession::Connect()
{
	this->m_pClient->Connect(this->m_pHost, this->m_pPort);

	this->m_pKeepAliveTimer->Reset();

	this->m_pReceiveHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CNetworkSession::ReceiveThread, this, 0, NULL);
}

void CNetworkSession::Disconnect()
{
	if(this->m_pClient != NULL)
		this->m_pClient->Disconnect();
}
#pragma endregion

#pragma region Overriden Methods
void CNetworkSession::Initialize()
{
	// create receive buffer
	this->m_iReceiveBufferSize = 200;
	this->m_iReceiveBufferUse = new int[this->m_iReceiveBufferSize];
	ZeroMemory(this->m_iReceiveBufferUse, sizeof(int) * this->m_iReceiveBufferSize);
	this->m_pReceiveBuffer = new char[this->m_iReceiveBufferSize * sizeof(Packet)];

	this->m_eState = NetworkSessionState::Lobby;

	this->m_pClient = new CMulticastClient();

	this->m_fKeepAliveTimeout = 5.0f;
	this->m_pKeepAliveTimer = new CGameTime();
}

void CNetworkSession::Update(CGameTime*)
{
	this->m_pClient->Flush();
}
#pragma endregion

#pragma region Static Methods
DWORD WINAPI CNetworkSession::ReceiveThread(LPVOID param)
{
	CNetworkSession* session = (CNetworkSession*)param;

	printf("[%s]Creating RECV thread\n", session->m_pIdentifier);

	WSAEVENT receiveEvent = WSACreateEvent();
	WSAEventSelect(session->m_pClient->Get_Socket(), receiveEvent, FD_READ);

	session->SendKeepAlive();

	int i = 0;
	while(true)
	{
		// sendout keepalive if one is needed
		session->SendKeepAlive();
	
		// read all of the packets in waiting
		int packets = 0;
		int count = 0;
		do
		{
			if((count = session->m_pClient->Receive(session->m_pReceiveBuffer, 0, sizeof(Packet))) > 0)
			{
				Packet* packet = (Packet*)session->m_pReceiveBuffer;
				if(packet->Command == PacketCommand::SendData)
				{
					D3DXVECTOR2* pos = (D3DXVECTOR2*)packet->Data;
					if(pos->x > 100 || pos->x < -100 || pos->y > 100 || pos->y < -100)
						printf("error");
					printf("[%s]Received packet:\n\tCommand:%i\n\tIdentifier: %s\n\tData: %f,%f\n", session->m_pIdentifier, packet->Command, packet->Identifier, pos->x, pos->y);
				}
				packets++;
			}

		} while(count > 0);

		if(packets > 0)
			printf("[%s]Received %i packets\n", session->m_pIdentifier, packets);

		Yield();
	}

	return 0;
}
#pragma endregion