#include "3ngine.Core.h"

enum NetworkSessionType : unsigned short { Local, SystemLink };
enum NetworkSessionState : unsigned short { Lobby, Playing, Ended };

struct CORE_API D3DX_SESSION_DESCRIPTION
{
	NetworkSessionType Type;
	char Identifier[32];
	unsigned short OpenSlots;
	unsigned short MaxPlayers;
	char Host[32];
	unsigned short Port;
};

class CNetworkSession;

class CORE_API INetworkService : public IGameService
{
	public:
		#pragma region Properties
		virtual CNetworkSession* CurrentSession() = 0;
		#pragma endregion

		#pragma region Methods
		virtual char* Get_Type();
		virtual CNetworkSession* CreateSession(NetworkSessionType, int) = 0;
		virtual std::map<std::string, D3DX_SESSION_DESCRIPTION> Find(NetworkSessionType) = 0;
		virtual std::map<std::string, float> NetworkedClients() = 0;
		virtual CNetworkSession* Join(D3DX_SESSION_DESCRIPTION) = 0;
		#pragma endregion
};