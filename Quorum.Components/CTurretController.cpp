/**
 * @description 
 *   Controls the ship's turrets; this class is used inconjunction with the CBaseCrewController.
 *   Once a controller is bound to the turret, the turret can fire bullets and rotate.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region IController Properties
//Return whether or not the controller is bound to a unit
bool CTurretController::Get_IsBound()
{
	return (this->m_pBoundUnit != NULL);
}

ControllerMode CTurretController::Get_Mode()
{
	return ControllerMode::None;
}

void CTurretController::Set_Mode(ControllerMode)
{
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CTurretController::CTurretController(CGame* game)
: CGameComponent(game)
{
	//Declare the controller variables
	this->m_pBoundUnit = NULL;
	this->m_pInputService = NULL;
}

//Deconstructor
CTurretController::~CTurretController()
{
	//Reset the input service and the bound unit to null
	this->m_pBoundUnit = NULL;
	this->m_pInputService = NULL;
}
#pragma endregion

#pragma region Methods
//Call the bound unit's fire method
void CTurretController::Fire()
{
	this->m_pBoundUnit->Fire();
}

//Call the bound unit's rotate method
void CTurretController::Rotate(float value)
{
	this->m_pBoundUnit->Rotate(value);
}

//Rotate the shield if there is a shield bound to the turret
void CTurretController::RotateShield(float value)
{
	this->m_pBoundUnit->RotateShield(value);
}

void CTurretController::Set_Color(int color)
{
	this->m_pBoundUnit->Set_Color(color);
}
#pragma endregion

#pragma region IController Methods
//Bind the controller to a turret object
void CTurretController::Bind(IControllableUnit* value)
{
	//Check to make sure whether or not the unit passed in is a turret object. If it is, then we bind it
	CTurret* component = dynamic_cast<CTurret*>(value);
	if(component != NULL)
	{
		if(this->m_pBoundUnit != NULL)
			this->m_pBoundUnit->Set_IsBound(false);

		this->m_pBoundUnit = component;
		this->m_pBoundUnit->Set_IsBound(true);
	}
}

//Unbind the controller from its current;y bound object
void CTurretController::Unbind()
{
	//Resetting the unit and the bound flag
	this->m_pBoundUnit->Set_IsBound(false);
	this->m_pBoundUnit = NULL;
}
#pragma endregion 

#pragma region Overriden Methods
//Initialize the controller
void CTurretController::Initialize()
{
	//Call CGameComponent's Initialize function
	CGameComponent::Initialize();
}

//Update the controller on every frame
void CTurretController::Update(CGameTime* gameTime)
{
	//Call CGameComponent's update function
	CGameComponent::Update(gameTime);
	
	//Check to make sure the turret is bound to an object
	if(this->m_pBoundUnit == NULL)
		return;
}
#pragma endregion