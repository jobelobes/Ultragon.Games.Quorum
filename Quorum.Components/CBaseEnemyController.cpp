/**
 * @description 
 *   Controls the enemy unit; at the moment this controller just determines
 *   a destination and moves towards it, then rechooses another destination.
 *   This will be replaced with a functional AI controller to manage the enemies
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
INetworkService* CBaseEnemyController::g_pNetwork = NULL;
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CBaseEnemyController::CBaseEnemyController(CGame* game)
:CBasicUnitController(game)
{

}

//Deconstructor
CBaseEnemyController::~CBaseEnemyController(){}
#pragma endregion

#pragma region Methods
void CBaseEnemyController::NetworkUpdate(CGameTime* gameTime, int identifier)
{
	if(CBaseEnemyController::g_pNetwork == NULL)
		return;

	CNetworkSession* session = CBaseEnemyController::g_pNetwork->Get_CurrentSession();
	if(session != NULL && session->Get_IsHost())
	{
		while(session->GetNextPacket(identifier).Command != PacketCommand::Empty);
		CEnemyUnit* enemy = (CEnemyUnit*)this->Get_BoundObject();
		
		ENEMY_PACKET enemyPacket;
		enemyPacket.Position = enemy->Get_Position();
		enemyPacket.Rotation = enemy->Get_Rotation();
		enemyPacket.Health = enemy->Get_Health();

		Packet packet;
		packet.Command = PacketCommand::SendData;
		packet.Identifier = identifier;
		packet.SetData((char*)&enemyPacket, 0, sizeof(ENEMY_PACKET));
		session->SendPacket(packet);
	}
}
#pragma endregion

#pragma region Overriden Methods
//Bind the controller to an enemy unit
void CBaseEnemyController::Bind(IControllableUnit* value)
{
	//Check to make sure the passed in unit is an enemy unit. If it is, then bind it
	CEnemyUnit* component = dynamic_cast<CEnemyUnit*>(value);
	if(component != NULL)
		CBasicUnitController::Bind(value);
}
//Initialize the controller
void CBaseEnemyController::Initialize()
{
	//Call CBasicUnitController's initialize function
	CBasicUnitController::Initialize();

	if(CBaseEnemyController::g_pNetwork == NULL)
		CBaseEnemyController::g_pNetwork = (INetworkService*)this->Get_Game()->Get_Services()->GetService("INetworkService");
}
#pragma endregion