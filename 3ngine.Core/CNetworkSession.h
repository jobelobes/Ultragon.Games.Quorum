#include "3ngine.Core.h"

		class CORE_API CNetworkSession : public CGameComponent
		{
			private:
				#pragma region Variables
				CRITICAL_SECTION m_sCritSection;
				unsigned long m_iThreadId;

				bool m_bIsHost;
				char* m_pIdentifier;
				unsigned short int m_iPlayerId;
				D3DX_SESSION_DESCRIPTION m_pGameDesc;

				std::map<std::string, float> m_pParticipants;

				CMulticastClient* m_pClient;
				HANDLE m_pReceiveHandle;
				
				CGameTime* m_pKeepAliveTimer;
				float m_fKeepAliveTimeout;

				int m_iPacketBufferSize;
				int* m_iPacketBufferUse;
				char* m_pPacketBuffer;

				int m_iReceiveBufferSize;
				int m_iSendBufferSize;
				#pragma endregion

				#pragma region Constructors and Finalizers
				CNetworkSession(CGame*, char*, char*, char*, int PacketBufferSize = 512, int recieveBufferSize = 4096, int sendBufferSize = 4096);
				CNetworkSession(CGame*, D3DX_SESSION_DESCRIPTION, char*, int PacketBufferSize = 512, int recieveBufferSize = 4096, int sendBufferSize = 4096);
				#pragma endregion 

				#pragma region Methods
				void UpdateClients(Packet Packet);
				void SendKeepAlive();
				#pragma endregion

				#pragma region Static Methods
				static DWORD WINAPI ReceiveThread(LPVOID param);
				#pragma endregion

			protected:
				#pragma region Properties
				CMulticastClient* Get_MulticastClient();
				#pragma endregion

			public:
				#pragma region Constructors and Finalizers
				~CNetworkSession();
				#pragma endregion 

				#pragma region Properties
				float Get_KeepAlive();
				void Set_KeepAlive(float);

				D3DX_SESSION_DESCRIPTION Get_GameDescription();

				bool Get_IsHost();

				int Get_PlayerIndex();
				void Set_PlayerIndex(int);

				std::map<std::string, float> Get_Participants();
				#pragma endregion

				#pragma region Methods
				bool IsDataAvailable();
				bool IsDataAvailable(char*);
				void ResetReady();
				void StartGame();
				void DestroyGame();
				void SetToHost();

				void ClearPackets();
				void ClearPackets(int);

				Packet GetNextPacket();
				Packet GetNextPacket(int);

				void SendPacket(Packet Packet);
				void Flush();

				void Connect();
				void Disconnect();
				#pragma endregion

				#pragma region Overriden Methods
				virtual void Initialize();
				virtual void Update(CGameTime*);
				#pragma endregion

				friend class CGameNetworkManager;
		};