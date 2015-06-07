#include "Quorum.Components.h"

#pragma region Static Variables
CQuorumSceneManager* CQuorumNetworkManager::g_pSceneManager = NULL;
#pragma endregion

#pragma region Methods
void CQuorumNetworkManager::ProcessPacket(Packet packet)
{
	CGameNetworkManager::ProcessPacket(packet);

	CNetworkSession* currentSession = this->Get_CurrentSession();
	if(currentSession != NULL)
	{
		switch(packet.Command)
		{
			case PacketCommand::StartGame:
				{
					D3DX_SESSION_DESCRIPTION session = currentSession->Get_GameDescription();
					printf("Creating game:\n\tIdentifier:%i\n\tMax Players:%i\n\tHost:%s:%i\n", session.Identifier, session.MaxPlayers, session.Host, session.Port);
					CQuorumNetworkManager::g_pSceneManager->CreateGame(currentSession->Get_IsHost(), currentSession->Get_GameDescription(), currentSession->Get_PlayerIndex());
					break;
				}
			case PacketCommand::DestroyGame:
				{
					D3DX_SESSION_DESCRIPTION session = currentSession->Get_GameDescription();
					printf("Destroying game:\n\tIdentifier:%i\n\tMax Players:%i\n\tHost:%s:%i\n", session.Identifier, session.MaxPlayers, session.Host, session.Port);
					CQuorumNetworkManager::g_pSceneManager->DestroyGame();
					break;
				}
		}
	}
}
#pragma endregion

#pragma region Constructors and Finalizers
CQuorumNetworkManager::CQuorumNetworkManager(CGame* game, char* host, char* port, char* identifier)
:CGameNetworkManager(game, host, port, identifier) {}
#pragma endregion

#pragma region Overriden Methods
void CQuorumNetworkManager::Initialize()
{
	CGameNetworkManager::Initialize();

	if(CQuorumNetworkManager::g_pSceneManager == NULL)
		CQuorumNetworkManager::g_pSceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
}
#pragma endregion