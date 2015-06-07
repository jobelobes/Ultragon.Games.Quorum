/**
 * @description 
 *   Controls the crew member and provides the basic controls for moving
 *   around the ship and controlling the turrets
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
INetworkService* CBaseCrewController::g_pNetwork = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
//Default constructor
CBaseCrewController::CBaseCrewController(CGame* game)
:CGameComponent(game)
{
	//Declare the controller's variables
	this->m_pBoundUnit = NULL;

	//Create the sub controller's for the crew controller
	this->m_pShipController = new CBasicUnitController(game);
	this->m_pTurretController = new CTurretController(game);
}

//Deconstructor
CBaseCrewController::~CBaseCrewController()
{
	//Reset the sub controllers
	if(this->m_pShipController != NULL)
		delete this->m_pShipController;
	if(this->m_pTurretController != NULL)
		delete this->m_pTurretController;
}
#pragma endregion

#pragma region Properties
//Return the controller's bound unit
CCrew* CBaseCrewController::Get_BoundUnit()
{
	return this->m_pBoundUnit;
}

//Return the controller's ship controller
CBasicUnitController* CBaseCrewController::Get_ShipController()
{
	return this->m_pShipController;
}

//Return the controller's turret controller
CTurretController* CBaseCrewController::Get_TurretController()
{
	return this->m_pTurretController;
}
#pragma endregion

#pragma region IController Properties
//Return whether the controller has a bound unit or not
bool CBaseCrewController::Get_IsBound()
{
	return this->m_pBoundUnit != NULL;
}

ControllerMode CBaseCrewController::Get_Mode()
{
	return this->m_eMode;
}

void CBaseCrewController::Set_Mode(ControllerMode value)
{
	this->m_eMode = value;
}
#pragma endregion

#pragma region IController Methods
//Bind the controller to a unit
void CBaseCrewController::Bind(IControllableUnit* value)
{
	//Check to make sure the passed in unit is a crew member, if it is then bind it to the controller
	CCrew* component = dynamic_cast<CCrew*>(value);
	if(component != NULL)
	{
		if(this->m_pBoundUnit != NULL)
			this->m_pBoundUnit->Set_IsBound(false);

		this->m_pBoundUnit = component;
		this->m_pBoundUnit->Set_IsBound(true);
	}
}

//Unbind the controller from its currently bound unit
void CBaseCrewController::Unbind()
{
	if(this->m_pTurretController->Get_IsBound())
			this->m_pTurretController->Unbind();

	if(this->m_pShipController->Get_IsBound())
		this->m_pShipController->Unbind();

	if(this->m_pBoundUnit->Get_IsBound())
		this->m_pBoundUnit->Set_IsBound(false);

	this->m_pBoundUnit = NULL;
}
#pragma endregion

#pragma region CBaseCrewController Methods
//Call the turret controller's fire function
void CBaseCrewController::Fire()
{
	this->m_pTurretController->Fire();
}

//Call the ship controller's move function
void CBaseCrewController::Move(D3DXVECTOR2 offset)
{
	this->m_pShipController->Move(offset);
}

//Call either the turret controller or ship controller's rotate function. 
//Determined by the type of station passed in
void CBaseCrewController::Rotate(float angle, StationType type)
{
	switch (type)
	{
		case Turret :
			this->m_pTurretController->Rotate(angle);
			break;
		case Pilot :
			this->m_pShipController->Rotate(angle);
			break;
		case Shield:
			this->m_pTurretController->RotateShield(angle);
			break;
	}
}

void CBaseCrewController::Set_Color(int color)
{
	this->m_pTurretController->Set_Color(color);
}

//Switch the station based on the passed in index
void CBaseCrewController::SwitchStation(int index)
{
	//check to see if the current crew member is bound to anything, unbind him and all the controllers
	if(this->m_pBoundUnit->Get_IsStationBound())
	{		
		if(this->m_pTurretController->Get_IsBound())
			this->m_pTurretController->Unbind();

		if(this->m_pShipController->Get_IsBound())
			this->m_pShipController->Unbind();	
	}

	//Pilot station
	if(index == 0)
		this->m_pShipController->Bind(this->m_pBoundUnit->Get_Ship());
	else
		this->m_pTurretController->Bind(this->m_pBoundUnit->Get_Ship()->GetTurretAt(index));

	this->m_pBoundUnit->Set_BoundStationIndex(index);
	
	if(this->m_pBoundUnit->Get_BoundStation() != NULL)
		//Set the crew member's world transform to the currently bound station's transform
		this->m_pBoundUnit->Set_Transform(this->m_pBoundUnit->Get_BoundStation()->Get_Transform());
}

void CBaseCrewController::NetworkUpdate(CGameTime* gameTime, int identifier)
{
	if(CBaseCrewController::g_pNetwork == NULL)
		return;

	CNetworkSession* session = CBaseCrewController::g_pNetwork->Get_CurrentSession();
	if(session != NULL)
	{
		while(session->GetNextPacket(identifier).Command != PacketCommand::Empty);

		CCrew* crew = this->Get_BoundUnit();
		if(crew->Get_BoundStationIndex() == -1)
			return;

		CStation* station = crew->Get_BoundStation();

		CREW_PACKET crewPacket;
		crewPacket.Index = crew->Get_BoundStationIndex();
		crewPacket.Type = station->Get_StationType();
		crewPacket.Position = station->Get_Ship()->Get_Position();
		if(station->Get_StationType() == StationType::Turret)
			crewPacket.Rotation = station->Get_Turret()->Get_BarrelRotation();
		else if(station->Get_StationType() == StationType::Pilot)
			crewPacket.Rotation = station->Get_Ship()->Get_Rotation();

		Packet packet;
		packet.Command = PacketCommand::SendData;
		packet.Identifier = identifier;
		packet.SetData((char*)&crewPacket, 0, sizeof(CREW_PACKET));
		session->SendPacket(packet);
	}
}
#pragma endregion

#pragma region Overriden Methods
void CBaseCrewController::Initialize()
{
	if(CBaseCrewController::g_pNetwork == NULL)
		CBaseCrewController::g_pNetwork = (INetworkService*)this->Get_Game()->Get_Services()->GetService("INetworkService");

}
#pragma endregion