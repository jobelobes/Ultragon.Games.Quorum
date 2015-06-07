/**
 * @description 
 *   An extension of the CBaseEnemyController, this class is the network's primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Overriden Properties
ControllerMode CNetworkEnemyController::Get_Mode()
{
	return ControllerMode::Network;
}
#pragma endregion

#pragma region Constructors and Deconstructors
/**
 * Default User Controller Constructor
 *
 * @param	game	current game object
 */
CNetworkEnemyController::CNetworkEnemyController(CGame* game)
: CBaseEnemyController(game) {}

/**
 * User Controller Deconstructor
 */
CNetworkEnemyController::~CNetworkEnemyController() {}
#pragma endregion

#pragma region Overriden Methods
/**
 * Derived Update method from the CGameComponent.
 *
 * @param	gameTime	the games elapsed/total time
 */
void CNetworkEnemyController::Update(CGameTime* gameTime)
{
	if(CNetworkEnemyController::g_pNetwork == NULL)
		return;

	CNetworkSession* session = CNetworkEnemyController::g_pNetwork->Get_CurrentSession();
	if(session != NULL && !session->Get_IsHost())
	{
		int identifier = ((CEnemyUnit*)this->Get_BoundObject())->Get_Identifier();

		Packet packet;
		while((packet = session->GetNextPacket(identifier)).Command != PacketCommand::Empty)
		{
			if(packet.Command != PacketCommand::SendData)
				continue;

			ENEMY_PACKET* enemyPacket = (ENEMY_PACKET*)packet.Data;
			CEnemyUnit* enemy = (CEnemyUnit*)this->Get_BoundObject();
			if(!enemy->Get_Visible())
			{
				enemy->Set_Enabled(true);
				enemy->Set_Visible(true);
			}
				
			enemy->Set_Position(enemyPacket->Position);
			enemy->Set_Rotation(enemyPacket->Rotation);
			enemy->Set_Health(enemyPacket->Health);
		}		
	}
}
#pragma endregion