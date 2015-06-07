#include "3ngine.Core.h"

		class CORE_API CGameNetworkManager : public CGameComponent, public INetworkService
		{
			private:
				#pragma region Variables
				char* m_pHost;
				char* m_pPort;
				char* m_pIdentifier;

				int m_iPacketBufferSize;
				int m_iReceiveBufferSize;
				int m_iSendBufferSize;

				float m_fSessionUpdateElapsed;
				CNetworkSession* m_pBackgroundSession;
				CNetworkSession* m_pCurrentSession;

				std::map<unsigned int, D3DX_SESSION_DESCRIPTION> m_pGameSessions;
				#pragma endregion

				#pragma region Methods
				void UpdateClients(Packet Packet, CGameTime* gameTime);
				void CreateGame(Packet Packet, CGameTime* gameTime);
				void StartGame(Packet Packet, CGameTime* gameTime);
				void UpdateGame(Packet Packet, CGameTime* gameTime);
				void DestroyGame(Packet Packet, CGameTime* gameTime);
				void JoinGame(Packet Packet, CGameTime* gameTime);
				#pragma endregion
			
			protected:
				#pragma region Methods
				virtual void ProcessPacket(Packet);
				#pragma endregion

			public:
				#pragma region INetworkService Properties
				virtual CNetworkSession* Get_CurrentSession();
				#pragma endregion

				#pragma region Constructors and Finalizers
				CGameNetworkManager(CGame*, char*, char*, int PacketBufferSize = 512, int recieveBufferSize = 4096, int sendBufferSize = 4096);
				CGameNetworkManager(CGame*, char*, char*, char*, int PacketBufferSize = 512, int recieveBufferSize = 4096, int sendBufferSize = 4096);
				virtual ~CGameNetworkManager();
				#pragma endregion

				#pragma region INetworkService Methods
				virtual CNetworkSession* CreateSession(NetworkSessionType type, int);
				virtual std::map<unsigned int, D3DX_SESSION_DESCRIPTION> Find(NetworkSessionType type);
				virtual std::map<std::string, float> NetworkedClients();
				virtual CNetworkSession* Join(D3DX_SESSION_DESCRIPTION session);
				#pragma endregion

				#pragma region Overriden Methods
				virtual void Initialize();
				virtual void Update(CGameTime*);
				#pragma endregion
		};