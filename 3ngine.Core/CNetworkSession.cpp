#include "3ngine.Core.h"


		#pragma region Properties
		D3DX_SESSION_DESCRIPTION CNetworkSession::Get_GameDescription()
		{
			return this->m_pGameDesc;
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

		int CNetworkSession::Get_PlayerIndex()
		{
			return this->m_iPlayerId;
		}

		void CNetworkSession::Set_PlayerIndex(int value)
		{
			this->m_iPlayerId = value;
		}

		std::map<std::string, float> CNetworkSession::Get_Participants()
		{
			return this->m_pParticipants;
		}
		#pragma endregion

		#pragma region Constructors and Finalizers
		CNetworkSession::CNetworkSession(CGame* game, char* host, char* port, char* identifier, int PacketBufferSize, int recieveBufferSize, int sendBufferSize)
		:CGameComponent(game)
		{
			this->m_iThreadId = 0;
			
			this->m_iPacketBufferSize = PacketBufferSize;
			this->m_iReceiveBufferSize = recieveBufferSize;
			this->m_iSendBufferSize = sendBufferSize;

			this->m_pIdentifier = new char[strlen(identifier)];
			strcpy(this->m_pIdentifier, identifier);

			this->m_pGameDesc.State = NetworkSessionState::General;
			strcpy((char*)this->m_pGameDesc.Host, host);
			this->m_pGameDesc.Port = atoi(port);
			this->m_bIsHost = false;

			InitializeCriticalSection(&this->m_sCritSection);
		}

		CNetworkSession::CNetworkSession(CGame* game, D3DX_SESSION_DESCRIPTION gameDesc, char* identifier, int PacketBufferSize, int recieveBufferSize, int sendBufferSize)
		:CGameComponent(game)
		{
			this->m_iPacketBufferSize = PacketBufferSize;
			this->m_iReceiveBufferSize = recieveBufferSize;
			this->m_iSendBufferSize = sendBufferSize;

			this->m_pIdentifier = new char[strlen(identifier)];
			strcpy(this->m_pIdentifier, identifier);

			memcpy(&this->m_pGameDesc, &gameDesc, sizeof(D3DX_SESSION_DESCRIPTION));
			this->m_bIsHost = false;

			InitializeCriticalSection(&this->m_sCritSection);
		}

		CNetworkSession::~CNetworkSession()
		{
			DeleteCriticalSection(&this->m_sCritSection);

			if(this->m_pClient != NULL)
				delete this->m_pClient;
			if(this->m_pIdentifier != NULL)
				delete this->m_pIdentifier;
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
			for(int i = 0; i < this->m_iPacketBufferSize; i++)
			{
				if(this->m_iPacketBufferUse[i] == 0)
					continue;

				packet = (Packet*)(this->m_pPacketBuffer + i * sizeof(Packet));
				if(packet != NULL)
					return true;
			}
			return false;
		}

		bool CNetworkSession::IsDataAvailable(char* identifier)
		{
			Packet* packet = NULL;
			for(int i = 0; i < this->m_iPacketBufferSize; i++)
			{
				if(this->m_iPacketBufferUse[i] == 0)
					continue;

				packet = (Packet*)(this->m_pPacketBuffer + i * sizeof(Packet));
				if(strcmp((char*)packet->Identifier, identifier) == 0 && packet != NULL)
					return true;
			}
			return false;
		}

		void CNetworkSession::UpdateClients(Packet Packet)
		{
			std::string key = Packet.Data;
			std::map<std::string, float>::iterator it;
			bool found = false;

			for ( it = this->m_pParticipants.begin() ; it != this->m_pParticipants.end(); it++ )
			{
				if((*it).first.compare(key) == 0)
				{
					(*it).second = 0;
					found = true;
				}
				else if((*it).second > 1000)
					this->m_pParticipants.erase(it);
				else
					(*it).second += 1;
			}

			// add the machine into the cache
			if(!found)
			{
				this->m_pParticipants[key] = 0;
				this->SendKeepAlive();
			}
		}

		void CNetworkSession::ResetReady()
		{
			if(this->m_pGameDesc.State != NetworkSessionState::Lobby)
				return;

			this->SendPacket(Packet(PacketCommand::ResetReady, PacketIdentifier::Broadcast, (char*)&this->m_pGameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
		}

		void CNetworkSession::StartGame()
		{
			if(!this->m_bIsHost)
				return;

			if(this->m_pGameDesc.State != NetworkSessionState::Lobby)
				return;

			this->m_pGameDesc.State = NetworkSessionState::Playing;
			this->m_pGameDesc.OpenSlots = this->m_pGameDesc.MaxPlayers - this->m_pParticipants.size();
			this->SendPacket(Packet(PacketCommand::StartGame, PacketIdentifier::Broadcast, (char*)&this->m_pGameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
			this->Flush();
		}

		void CNetworkSession::DestroyGame()
		{
			if(!this->m_bIsHost)
				return;

			if(this->m_pGameDesc.State != NetworkSessionState::Lobby)
				return;

			this->m_pGameDesc.State = NetworkSessionState::Playing;
			this->m_pGameDesc.OpenSlots = this->m_pGameDesc.MaxPlayers - this->m_pParticipants.size();
			this->SendPacket(Packet(PacketCommand::DestroyGame, PacketIdentifier::Broadcast, (char*)&this->m_pGameDesc, sizeof(D3DX_SESSION_DESCRIPTION)));
			this->Flush();
		}

		void CNetworkSession::ClearPackets()
		{
			this->ClearPackets(-1);
		}

		void CNetworkSession::ClearPackets(int identifier)
		{
			EnterCriticalSection(&this->m_sCritSection);
			{
				Packet* packet = NULL;
				for(int i = 0; i < this->m_iPacketBufferSize; i++)
				{
					if(this->m_iPacketBufferUse[i] == 0)
						continue;

					packet = (Packet*)(this->m_pPacketBuffer + i * sizeof(Packet));
					if(packet->Identifier == identifier || identifier == -1)
						this->m_iPacketBufferUse[i] = 0;
				}
			}
			LeaveCriticalSection(&this->m_sCritSection);
		}

		Packet CNetworkSession::GetNextPacket(int identifier)
		{
			int index = -1;
			Packet* packet = NULL;
			for(int i = 0; i < this->m_iPacketBufferSize; i++)
			{
				if(this->m_iPacketBufferUse[i] == 0)
					continue;
			
				packet = (Packet*)(this->m_pPacketBuffer + i * sizeof(Packet));
				if(index == -1 || this->m_iPacketBufferUse[i] > this->m_iPacketBufferUse[index])
				{
					if(packet->Identifier == identifier)
						index = i;
				}
			}
			if(index == -1 || index >= this->m_iPacketBufferSize)
				return Packet();

			EnterCriticalSection(&this->m_sCritSection);
			{
				this->m_iPacketBufferUse[index] = 0;
			}
			LeaveCriticalSection(&this->m_sCritSection);

			return *(Packet*)(this->m_pPacketBuffer + index * sizeof(Packet));
		}

		Packet CNetworkSession::GetNextPacket()
		{
			return this->GetNextPacket(PacketIdentifier::Broadcast);
		}

		void CNetworkSession::SendKeepAlive()
		{
			this->m_pKeepAliveTimer->Tick();
			if(this->m_pKeepAliveTimer->Get_TotalTime() > this->m_fKeepAliveTimeout)
			{
				this->SendPacket(Packet(PacketCommand::KeepAlive, PacketIdentifier::Broadcast, this->m_pIdentifier, strlen(this->m_pIdentifier) + 1));
				this->m_pKeepAliveTimer->Reset();

				#ifdef _DEBUG
				printf("Sent keep alive Packet\n");
				#endif
			}
		}

		void CNetworkSession::SendPacket(Packet Packet)
		{
			this->m_pClient->Send((char*)&Packet, 0, sizeof(Packet));
		}

		void CNetworkSession::Flush()
		{
			this->m_pClient->Flush();
		}

		void CNetworkSession::Connect()
		{
			char port[6];
			itoa(this->m_pGameDesc.Port, port, 10);

			this->m_pClient->Connect(this->m_pGameDesc.Host, port);

			this->m_pKeepAliveTimer->Reset();

			this->m_pReceiveHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CNetworkSession::ReceiveThread, this, 0, &this->m_iThreadId);

			#ifdef _DEBUG
			printf("Connecting to game hosted on %s:%i\n", this->m_pGameDesc.Host, this->m_pGameDesc.Port);
			#endif
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
			this->m_iPacketBufferUse = new int[this->m_iPacketBufferSize];
			ZeroMemory(this->m_iPacketBufferUse, sizeof(int) * this->m_iPacketBufferSize);
			this->m_pPacketBuffer = new char[this->m_iPacketBufferSize * sizeof(Packet)];

			this->m_pGameDesc.State = NetworkSessionState::Lobby;

			this->m_pClient = new CMulticastClient(this->m_iReceiveBufferSize, this->m_iSendBufferSize);

			this->m_fKeepAliveTimeout = 5.0f;
			this->m_pKeepAliveTimer = new CGameTime();
		}

		void CNetworkSession::Update(CGameTime* gameTime)
		{
			CGameComponent::Update(gameTime);

			this->m_pClient->Flush();
		}
		#pragma endregion

		#pragma region Static Methods
		DWORD WINAPI CNetworkSession::ReceiveThread(LPVOID param)
		{
			CNetworkSession* session = (CNetworkSession*)param;

			#ifdef _DEBUG
			printf("[%s:%i]Starting RECV thread\n", session->m_pGameDesc.Host, session->m_pGameDesc.Port);
			#endif
			
			WSAEVENT receiveEvent = WSACreateEvent();
			WSAEventSelect(session->m_pClient->Get_Socket(), receiveEvent, FD_READ);

			int i = 0;
			while(true)
			{			
				// sendout keepalive if one is needed
				if(session->m_fKeepAliveTimeout > 0)
					session->SendKeepAlive();
			
				// read all of the Packets in waiting
				int count = 0;
				do
				{
					EnterCriticalSection(&session->m_sCritSection);
					{
						// find the next open buffer spot
						for(i = 0; i < session->m_iPacketBufferSize; i++)
						{
							// find open spots or really old spots
							if(session->m_iPacketBufferUse[i] == 0 || session->m_iPacketBufferUse[i] > 1000)
								break;
							session->m_iPacketBufferUse[i]++;
						}
						

						// no one is taking their Packets
						if(i == session->m_iPacketBufferSize)
							break;

						if((count = session->m_pClient->Receive(session->m_pPacketBuffer, i * sizeof(Packet), sizeof(Packet))) > 0)
						{
							Packet* packet = (Packet*)(session->m_pPacketBuffer + i * sizeof(Packet));
							if(packet->Command == PacketCommand::KeepAlive)
							{
								session->UpdateClients(*packet);

								#ifdef _DEBUG
								printf("Received keep alive Packet\n");
								#endif
							}
							else
								session->m_iPacketBufferUse[i] = 1;
						}
					}
					LeaveCriticalSection(&session->m_sCritSection);

				} while(count > 0);

				Sleep(33);
			}

			#ifdef _DEBUG
			printf("[%s:%i]Stopping RECV thread\n", session->m_pGameDesc.Host, session->m_pGameDesc.Port);
			#endif

			return 0;
		}
		#pragma endregion