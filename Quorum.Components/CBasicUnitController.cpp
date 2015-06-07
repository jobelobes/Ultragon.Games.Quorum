/**
 * @description 
 *   Controls a basic unit in the game; this class is a base class that 
 *   other controllers use to extend how a unit is controlled
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region IController Properties
//Returns if the controller has a bound unit or not
bool CBasicUnitController::Get_IsBound()
{
	return (this->m_pBoundUnit != NULL);
}

ControllerMode CBasicUnitController::Get_Mode()
{
	return this->m_eMode;
}

void CBasicUnitController::Set_Mode(ControllerMode value)
{
	this->m_eMode = value;
}
#pragma endregion

#pragma region Properties

//Returns the controller's bound unit
IBasicUnit* CBasicUnitController::Get_BoundObject()
{
	return this->m_pBoundUnit;
}


#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CBasicUnitController::CBasicUnitController(CGame* game)
: CGameComponent(game)
{
	//Initialize the bound unit
	this->m_pBoundUnit = NULL;
}

//Deconstructor
CBasicUnitController::~CBasicUnitController()
{
	//Reset the bound unit to null
	this->m_pBoundUnit = NULL;
}
#pragma endregion

#pragma region Methods
//Moves the bound unit based on a vector offset
void CBasicUnitController::Move(D3DXVECTOR2 offset)
{
	//Calls the bound unit's move function
	this->m_pBoundUnit->Move(offset);
}

//Rotates the bound unit based on an angle
void CBasicUnitController::Rotate(float value)
{
	//Calls the bound unit's rotate function
	this->m_pBoundUnit->Rotate(value);
}

//RFires at the given target
void CBasicUnitController::Fire(D3DXVECTOR2 dir)
{
	//Calls the bound unit's fire function
	this->m_pBoundUnit->Fire(dir);
}

#pragma endregion

#pragma region IController Methods
//Bind the controller to a unit
void CBasicUnitController::Bind(IControllableUnit* value)
{
	//Cast the passed in unit to an IBasicUnit to check if its valid
	IBasicUnit* component = dynamic_cast<IBasicUnit*>(value);
	if(component != NULL)
		this->m_pBoundUnit = component;
}

//Unbind the controller from its currently bound unit
void CBasicUnitController::Unbind()
{
	//Reset the bound unit
	this->m_pBoundUnit = NULL;
}
#pragma endregion 

#pragma region Overriden Methods
//Initialize the controller
void CBasicUnitController::Initialize()
{
	//Call CGameComponent's initialize function
	CGameComponent::Initialize();
}

//Update the controller's state
void CBasicUnitController::Update(CGameTime* gameTime)
{
	//Call the CGameComponent's Update function
	CGameComponent::Update(gameTime);

	//Check if the bound unit is null
	if(this->m_pBoundUnit == NULL)
		return;
}
#pragma endregion