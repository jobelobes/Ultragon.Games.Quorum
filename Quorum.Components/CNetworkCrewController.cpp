/**
 * @description 
 *   An extension of the CBaseCrewController, this class is the network's primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Overriden Properties
ControllerMode CNetworkCrewController::Get_Mode()
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
CNetworkCrewController::CNetworkCrewController(CGame* game)
: CBaseCrewController(game) {}

/**
 * User Controller Deconstructor
 */
CNetworkCrewController::~CNetworkCrewController() {}
#pragma endregion

#pragma region Overriden Methods
/**
 * Derived Update method from the CGameComponent.
 *
 * @param	gameTime	the games elapsed/total time
 */
void CNetworkCrewController::Update(CGameTime* gameTime)
{
	if(CNetworkCrewController::g_pNetwork == NULL)
		return;

	if(this->Get_BoundUnit() == NULL)
		return;

	CNetworkSession* session = CNetworkCrewController::g_pNetwork->Get_CurrentSession();
	if(session != NULL)
	{
		int identifier = this->Get_BoundUnit()->Get_Identifier();

		Packet packet;
		while((packet = session->GetNextPacket(identifier)).Command != PacketCommand::Empty)
		{
			if(packet.Command != PacketCommand::SendData)
				continue;

			CCrew* crew = this->Get_BoundUnit();
			if(!crew->Get_Visible())
			{
				crew->Set_Enabled(true);
				crew->Set_Visible(true);
			}

			CREW_PACKET* crewPacket = (CREW_PACKET*)packet.Data;
			if(crew->Get_BoundStationIndex() != crewPacket->Index)
				this->SwitchStation(crewPacket->Index);	

			CStation* station = crew->Get_BoundStation();
			switch(crewPacket->Type)
			{
				case StationType::Pilot:
				{
					station->Get_Ship()->Set_Position(crewPacket->Position);
					station->Get_Ship()->Set_Rotation(crewPacket->Rotation);
					break;
				}
				case StationType::Turret:
				{
					station->Get_Turret()->Set_BarrelRotation(crewPacket->Rotation);
					break;
				}
			}			
		}		
	}
}
#pragma endregion