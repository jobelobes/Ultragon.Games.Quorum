#include "3ngine.Core.h"

class CORE_API CGameNetworkManager : public CGameComponent, public INetworkService
{
	private:
		#pragma region Variables
		char* m_pHost;
		char* m_pPort;
		char* m_pIdentifier;

		CNetworkSession* m_pBackgroundSession;
		CNetworkSession* m_pCurrentSession;

		std::map<std::string, D3DX_SESSION_DESCRIPTION> m_pGameSessions;
		std::map<std::string, float> m_pMachines;
		#pragma endregion

		#pragma region Methods
		void UpdateClients(Packet, CGameTime*);
		void CreateGame(Packet, CGameTime*);
		void UpdateGame(Packet, CGameTime*);
		void DestroyGame(Packet, CGameTime*);
		void JoinGame(Packet, CGameTime*);
		#pragma endregion
		
	public:
		#pragma region INetworkService Properties
		virtual CNetworkSession* CurrentSession();
		#pragma endregion

		#pragma region Constructors and Finalizers
		CGameNetworkManager(CGame*, char*, char*, char*);
		~CGameNetworkManager();
		#pragma endregion

		#pragma region INetworkService Methods
		virtual CNetworkSession* CreateSession(NetworkSessionType, int);
		virtual std::map<std::string, D3DX_SESSION_DESCRIPTION> Find(NetworkSessionType);
		virtual std::map<std::string, float> NetworkedClients();
		virtual CNetworkSession* Join(D3DX_SESSION_DESCRIPTION);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion
};