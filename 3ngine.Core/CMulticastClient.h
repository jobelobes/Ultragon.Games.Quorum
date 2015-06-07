#include "3ngine.Core.h"

		class CORE_API CGameNetworkManager;

		class CORE_API CMulticastClient
		{
			private:
				#pragma region Static Variables
				static int g_iReferences;
				#pragma endregion

				#pragma region Variables
				bool m_bConnected;
				bool m_bAutoFlush;

				ADDRINFO* m_sMulticastAddr;
				ADDRINFO* m_sLocalAddr;
				SOCKET m_sSocket;

				int m_iReceiveBufferSize;
				int m_iReceiveBufferOffset;
				char* m_pReceiveBuffer;

				int m_iSendBufferSize;
				int m_iSendBufferOffset;
				char* m_pSendBuffer;
				#pragma endregion

				#pragma region Methods
				bool CreateSocket(ADDRINFO* info);

				bool JoinMulticast(ADDRINFO*);
				bool DropMulticast(ADDRINFO*);

				void SendKeepAlive();
				#pragma endregion

				#pragma region Static Methods
				bool InitializeSockets();

				static DWORD WINAPI ReceiveThread(LPVOID);
				#pragma endregion

			public:
				#pragma region Properties
				bool Get_IsConnected();
				
				SOCKET Get_Socket();

				int Get_ReceiveBufferSize();
				void Set_ReceiveBufferSize(int);

				int Get_SendBufferSize();
				void Set_SendBufferSize(int);

				bool Get_AutoFlush();
				void Set_AutoFlush(bool);
				#pragma endregion

				#pragma region Constructors and Finalizers
				CMulticastClient(int receiveBufferSize = 4096, int sendBufferSize = 4096);
				~CMulticastClient();
				#pragma endregion

				#pragma region Methods
				bool Connect(char*, char*);
				void Disconnect();
				void Flush();

				int Send(char*, int, int);
				int Receive(char*, int, int);
				#pragma endregion

				friend class CNetworkSession;
		};
