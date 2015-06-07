#include "Quorum.Components.h"

#pragma region Constructors and Deconstructors
//Default constructor
CShieldController::CShieldController(CGame* game)
: CGameComponent(game)
{
	//Declare the controller variables
	this->m_pBoundUnit = NULL;
	IInputService* m_pInputService = NULL;
}

//Deconstructor
CShieldController::~CShieldController()
{
	//Reset the input service and the bound unit to null
	this->m_pBoundUnit = NULL;
	this->m_pInputService = NULL;
}
#pragma endregion

#pragma region IController Properties
//Return whether or not the controller is bound to a unit
bool CShieldController::Get_IsBound()
{
	return (this->m_pBoundUnit != NULL);
}
#pragma endregion

#pragma region IController Methods
//Bind the controller to a turret object
void CShieldController::Bind(IControllableUnit* value)
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
void CShieldController::Unbind()
{
	//Resetting the unit and the bound flag
	this->m_pBoundUnit->Set_IsBound(false);
	this->m_pBoundUnit = NULL;
}
#pragma endregion

#pragma region Methods
//Call the bound unit's rotate method
void CShieldController::Rotate(float value)
{
	this->m_pBoundUnit->Rotate(value);
}

void CShieldController::Set_Color(int value)
{
	this->m_pBoundUnit->Set_Color(value);
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the controller
void CShieldController::Initialize()
{
	//Call CGameComponent's Initialize function
	CGameComponent::Initialize();
}

//Update the controller on every frame
void CShieldController::Update(CGameTime* gameTime)
{
	//Call CGameComponent's update function
	CGameComponent::Update(gameTime);
	
	//Check to make sure the turret is bound to an object
	if(this->m_pBoundUnit == NULL)
		return;
}
#pragma endregion