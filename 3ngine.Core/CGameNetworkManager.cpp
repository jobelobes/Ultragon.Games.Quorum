#include "3ngine.Core.h"


		#pragma region INetworkService Properties
		CNetworkSession* CGameNetworkManager::Get_CurrentSession()
		{
			return this->m_pCurrentSession;
		}
		#pragma endregion

		#pragma region Constructors and Finalizers
		CGameNetworkManager::CGameNetworkManager(CGame* game, char* host, char* port, int PacketBufferSize, int recieveBufferSize, int sendBufferSize)
		: CGameComponent(game)
		{
			DWORD computerNameSize = MAX_COMPUTERNAME_LENGTH  + 1;
			char computerName[MAX_COMPUTERNAME_LENGTH + 1];
			GetComputerName(computerName, &computerNameSize);

			srand(time(NULL));
			this->m_pHost = host;
			this->m_pPort = port;
			this->m_pIdentifier = new char[strlen(computerName)];
			strcpy(this->m_pIdentifier, computerName);
			this->m_pCurrentSession = NULL;

			this->m_iPacketBufferSize = PacketBufferSize;
			this->m_iReceiveBufferSize = recieveBufferSize;
			this->m_iSendBufferSize = sendBufferSize;

			game->Get_Services()->AddService(this);
		}

		CGameNetworkManager::CGameNetworkManager(CGame* game, char* host, char* port, char* identifier, int PacketBufferSize, int recieveBufferSize, int sendBufferSize)
		: CGameComponent(game)
		{
			srand(time(NULL));
			this->m_pHost = host;
			this->m_pPort = port;
			this->m_pIdentifier = new char[strlen(identifier)];
			strcpy(this->m_pIdentifier, identifier);
			this->m_pCurrentSession = NULL;

			this->m_iPacketBufferSize = PacketBufferSize;
			this->m_iReceiveBufferSize = recieveBufferSize;
			this->m_iSendBufferSize = sendBufferSize;

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
		void CGameNetworkManager::CreateGame(Packet Packet, CGameTime* gameTime)
		{
			D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)Packet.Data;
			if(this->m_pGameSessions.find(session->Identifier) == this->m_pGameSessions.end())
			{
				//add the game to the cache
				D3DX_SESSION_DESCRIPTION* tmpSession = new D3DX_SESSION_DESCRIPTION();
				memcpy(tmpSession, session, sizeof(D3DX_SESSION_DESCRIPTION));
				this->m_pGameSessions[session->Identifier] = *tmpSession;
			}
		}

		void CGameNetworkManager::StartGame(Packet Packet, CGameTime* gameTime)
		{
			
		}

		void CGameNetworkManager::UpdateGame(Packet Packet, CGameTime* gameTime)
		{
			D3DX_SESSION_DESCRIPTION session;
			memcpy(&session, Packet.Data, sizeof(D3DX_SESSION_DESCRIPTION));
			this->m_pGameSessions[session.Identifier] = session;
		}

		void CGameNetworkManager::DestroyGame(Packet Packet, CGameTime* gameTime)
		{
			D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)Packet.Data;
			std::map<unsigned int, D3DX_SESSION_DESCRIPTION>::iterator it;
			if((it = this->m_pGameSessions.find(session->Identifier)) != this->m_pGameSessions.end())
				this->m_pGameSessions.erase(it);
		}

		void CGameNetworkManager::JoinGame(Packet packet, CGameTime* gameTime)
		{
			D3DX_SESSION_DESCRIPTION* session = (D3DX_SESSION_DESCRIPTION*)packet.Data;
			std::map<unsigned int, D3DX_SESSION_DESCRIPTION>::iterator it;
			if((it = this->m_pGameSessions.find(session->Identifier)) != this->m_pGameSessions.end())
			{
				D3DX_SESSION_DESCRIPTION gameSession = (*it).second;
				gameSession.OpenSlots--;

				Packet tmpPacket = Packet(PacketCommand::UpdateGame, PacketIdentifier::Broadcast, (char*)&gameSession, sizeof(D3DX_SESSION_DESCRIPTION));
				this->m_pBackgroundSession->SendPacket(tmpPacket);
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
				gameDesc.Identifier = this->m_pGameSessions.size() + 1;
				strcpy(gameDesc.Host,this->m_pHost);
				gameDesc.Port = rand() % 3000 + 3000;
				gameDesc.MaxPlayers = maxPlayers;
				gameDesc.OpenSlots = maxPlayers - 1;
				this->m_pGameSessions[gameDesc.Identifier] = gameDesc;

				int playerIndex = 1;
				char identifier[32];
				sprintf(identifier, "@id/game/%i/%i", gameDesc.Identifier, playerIndex);

				// send back the new sessions credentials
				this->m_pCurrentSession = new CNetworkSession(this->Get_Game(), gameDesc, identifier, this->m_iPacketBufferSize, this->m_iReceiveBufferSize, this->m_iSendBufferSize);
				this->Get_Game()->Get_Components()->Add(this->m_pCurrentSession);
				this->m_pCurrentSession->Set_PlayerIndex(playerIndex);
				this->m_pCurrentSession->SetToHost();
				this->m_pCurrentSession->Connect();

				// register the game
				this->m_pBackgroundSession->SendPacket(Packet(PacketCommand::CreateGame, PacketIdentifier::Broadcast, (char*)&gameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
				this->m_pBackgroundSession->Flush();
			}

			return this->m_pCurrentSession;
		}

		std::map<unsigned int, D3DX_SESSION_DESCRIPTION> CGameNetworkManager::Find(NetworkSessionType)
		{
			return this->m_pGameSessions;
		}

		std::map<std::string, float> CGameNetworkManager::NetworkedClients()
		{
			return this->m_pBackgroundSession->Get_Participants();
		}

		CNetworkSession* CGameNetworkManager::Join(D3DX_SESSION_DESCRIPTION description)
		{
			std::map<unsigned int, D3DX_SESSION_DESCRIPTION>::iterator it;
			if((it = this->m_pGameSessions.find(description.Identifier)) != this->m_pGameSessions.end())
			{
				D3DX_SESSION_DESCRIPTION gameDesc = (*it).second;
				if(gameDesc.OpenSlots > 0)
				{
					gameDesc.OpenSlots--;

					int playerIndex = gameDesc.MaxPlayers - gameDesc.OpenSlots;
					char identifier[32];
					sprintf(identifier, "@id/game/%i/%i",gameDesc.Identifier,playerIndex);

					// send back the new sessions credentials
					#ifdef _DEBUG
					printf("Setting up data network");
					#endif

					this->m_pCurrentSession = new CNetworkSession(this->Get_Game(), gameDesc, identifier, this->m_iPacketBufferSize, this->m_iReceiveBufferSize, this->m_iSendBufferSize);
					this->Get_Game()->Get_Components()->Add(this->m_pCurrentSession);
					this->m_pCurrentSession->Set_PlayerIndex(playerIndex);
					this->m_pCurrentSession->Connect();

					// register as a client
					this->m_pBackgroundSession->SendPacket(Packet(PacketCommand::JoinGame, PacketIdentifier::Broadcast, (char*)&gameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
					this->m_pBackgroundSession->Flush();
				}
			}
			return NULL;
		}

		void CGameNetworkManager::ProcessPacket(Packet Packet)
		{
			
		}
		#pragma endregion

		#pragma region Overriden Methods
		void CGameNetworkManager::Initialize()
		{
			this->m_fSessionUpdateElapsed = 0.0f;
			this->m_pBackgroundSession = new CNetworkSession(this->Get_Game(), this->m_pHost, this->m_pPort, this->m_pIdentifier, this->m_iPacketBufferSize, this->m_iReceiveBufferSize, this->m_iSendBufferSize);
			this->Get_Game()->Get_Components()->Add(this->m_pBackgroundSession);
			this->m_pBackgroundSession->Set_KeepAlive(5);
			this->m_pBackgroundSession->Connect();
		}

		void CGameNetworkManager::Update(CGameTime* gameTime)
		{
			if(this->m_pBackgroundSession == NULL)
				return;

			this->m_fSessionUpdateElapsed += gameTime->Get_ElapsedTime();
			
			// resend out the current running games
			if(this->m_fSessionUpdateElapsed > 10.0)
			{
				std::map<unsigned int, D3DX_SESSION_DESCRIPTION>::iterator it;
				for(it = this->m_pGameSessions.begin(); it != this->m_pGameSessions.end(); it++)
					this->m_pBackgroundSession->SendPacket(Packet(PacketCommand::UpdateGame, PacketIdentifier::Broadcast, (char*)&(*it).second, sizeof(D3DX_SESSION_DESCRIPTION)));

				this->m_fSessionUpdateElapsed = 0.0f;
			}


			Packet Packet;
			while((Packet = this->m_pBackgroundSession->GetNextPacket()).Command != PacketCommand::Empty)
			{
				switch(Packet.Command)
				{
					case PacketCommand::CreateGame:
					{
						#ifdef _DEBUG
						printf("Processing \"CreateGame\" Packet...\n");
						#endif

						this->CreateGame(Packet, gameTime);
						break;
					}
					case PacketCommand::DestroyGame:
					{
						#ifdef _DEBUG
						printf("Processing \"DestroyGame\" Packet...\n");
						#endif

						this->DestroyGame(Packet, gameTime);
						break;
					}
					case PacketCommand::UpdateGame:
					{
						#ifdef _DEBUG
						printf("Processing \"UpdateGame\" Packet...\n");
						#endif

						this->UpdateGame(Packet, gameTime);
						break;
					}
					case PacketCommand::JoinGame:
					{
						#ifdef _DEBUG
						printf("Processing \"JoinGame\" Packet...\n");
						#endif

						this->JoinGame(Packet, gameTime);
						break;
					}
				}

				this->ProcessPacket(Packet);
			}

			if(this->m_pCurrentSession == NULL)
				return;

			while((Packet = this->m_pCurrentSession->GetNextPacket()).Command != PacketCommand::Empty)
			{
				switch(Packet.Command)
				{
					case PacketCommand::StartGame:
					{
						#ifdef _DEBUG
						printf("Processing \"StartGame\" Packet...\n");
						#endif

						this->StartGame(Packet, gameTime);
						break;
					}
				}

				this->ProcessPacket(Packet);
			}
		}
		#pragma endregion