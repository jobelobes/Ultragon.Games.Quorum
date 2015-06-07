#include "3ngine.Core.h"

class CORE_API CNetworkSession : public CGameComponent
{
	private:
		#pragma region Variables
		char* m_pHost;
		char* m_pPort;
		char* m_pIdentifier;

		bool m_bIsHost;
		int m_iMaxGamers;
		CGameTime* m_pKeepAliveTimer;
		float m_fKeepAliveTimeout;

		NetworkSessionState m_eState;
		NetworkSessionType m_eType;
		
		int m_iReceiveBufferSize;
		int* m_iReceiveBufferUse;
		char* m_pReceiveBuffer;

		CMulticastClient* m_pClient;

		HANDLE m_pReceiveHandle;
		#pragma endregion

		#pragma region Constructors and Finalizers
		CNetworkSession(CGame*, char*, char*, char*);
		#pragma endregion 

		#pragma region Methods
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

		NetworkSessionState Get_State();

		bool Get_IsHost();
		#pragma endregion

		#pragma region Methods
		bool IsDataAvailable();
		bool IsDataAvailable(char*);
		void ResetReady();
		void StartGame();
		void SetToHost();

		void ClearPackets();
		void ClearPackets(char*);

		Packet GetNextPacket();
		Packet GetNextPacket(char*);

		void SendPacket(Packet*);

		void Connect();
		void Disconnect();
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion

		friend class CGameNetworkManager;
};