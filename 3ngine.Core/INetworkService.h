#include "3ngine.Core.h"

		enum NetworkSessionType : unsigned short { Local, SystemLink };
		enum NetworkSessionState : unsigned short { General, Lobby, Playing, Ended };

		struct D3DX_SESSION_DESCRIPTION
		{
			NetworkSessionType Type;
			NetworkSessionState State;

			unsigned int Identifier;
			unsigned short OpenSlots;
			unsigned short MaxPlayers;

			char Host[32];
			unsigned short Port;
		};

		class CORE_API CNetworkSession;

		class CORE_API INetworkService : public IGameService
		{
			public:
				#pragma region Properties
				virtual CNetworkSession* Get_CurrentSession() = 0;
				#pragma endregion

				#pragma region IGameService Methods
				virtual char* Get_Type() { return "INetworkService"; }
				#pragma endregion

				#pragma region Methods
				virtual CNetworkSession* CreateSession(NetworkSessionType, int) = 0;
				virtual std::map<unsigned int, D3DX_SESSION_DESCRIPTION> Find(NetworkSessionType) = 0;
				virtual std::map<std::string, float> NetworkedClients() = 0;
				virtual CNetworkSession* Join(D3DX_SESSION_DESCRIPTION) = 0;
				#pragma endregion
		};
