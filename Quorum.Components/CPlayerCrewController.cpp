/**
 * @description 
 *   An extension of the CBaseCrewController, this class is the users primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Constructors and Deconstructors
//Default constructor
CPlayerCrewController::CPlayerCrewController(CGame* game)
	:CBaseCrewController(game)
{
	//Declare the controller's variables
	this->m_iStationIndex = 0;
	this->m_pInputService = NULL;

	this->m_pTarget = NULL;
}

//Deconstructor
CPlayerCrewController::~CPlayerCrewController() {}
#pragma endregion

#pragma region Methods
//All windows input for the user while in the game state. Handles all station types and function
void CPlayerCrewController::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	float elapsedTime = gameTime->Get_ElapsedTime();
	
	//retrieve the bound unit and the station its bound to
	CCrew* boundCrew = this->Get_BoundUnit();
	CStation* station = boundCrew->Get_BoundStation();
	
	//if the station is null, by default, assign it to the pilot station
	/**
	 * Note by Eric Moreau: 
	 * will change in later versions to handle assigning a station to
	 * all crew members at the beginning of the game. Needs to check station occupancy
	 */
	if(station == NULL)
	{
		this->SwitchStation(0);

		this->CreateTarget();
		return;
	}

	//get the station type of the bound station
	StationType type = station->Get_StationType();

	//Switching to the pilot station
	if(inputService->IsPressed(DIK_LBRACKET) && inputService->IsPressed(DIK_RBRACKET))
	{

		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		this->SwitchStation(0);

		this->m_pTarget->Set_IsTurretBound(false);

		//unbind the target from its turret
		if(this->m_iStationIndex > 0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();
	}
	//Switching stations counter clockwise
	else if(inputService->IsPressed(DIK_LBRACKET))
	{
		// play the left sound
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);

		//unbind the current target
		if(this->m_iStationIndex >0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();

		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		//Adjusts the index of the station to move to
		this->m_iStationIndex += 1;
		if(this->m_iStationIndex > 6)
			this->m_iStationIndex = 1;

		//Call's switch station function with an adjusted index passed in
		this->SwitchStation(this->m_iStationIndex);

		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Turret)
		{
			//Sets the target's draw flag to true if the station type is a target
			this->m_pTarget->Set_IsTurretBound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->BindTarget(this->m_pTarget);
		}else{
			//Sets the target's draw flag to false if the station type is not a turret
			this->m_pTarget->Set_IsTurretBound(false);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();
		}
	}
	//Switching stations clockwise
	else if(inputService->IsPressed(DIK_RBRACKET))
	{
		// play the right sound
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);

		//unbind the current target
		if(this->m_iStationIndex >0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();

		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		//Adjusts the index of the station
		this->m_iStationIndex -= 1;
		if(this->m_iStationIndex < 1)
			this->m_iStationIndex = 6;

		//Calls switch station function with an adjusted index passed in
		this->SwitchStation(this->m_iStationIndex);

		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Turret)
		{
			//Sets the target's draw flag to true if the station type is a target
			this->m_pTarget->Set_IsTurretBound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->BindTarget(this->m_pTarget);
		}else{
			//Sets the target's draw flag to false if the station type is not a turret
			this->m_pTarget->Set_IsTurretBound(false);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();
		}
	}

	D3DXVECTOR2 offset;
	offset.x = 0;
	offset.y = 0;

	//Controls for the station if its a pilot type
	if(type == StationType::Pilot)
	{
		if(inputService->IsDown(DIK_RIGHT) || inputService->IsDown(DIK_LEFT))
			offset.x = (inputService->IsDown(DIK_LEFT)?1:-1) * elapsedTime * 0.5 * station->Get_MoveRate();
		if(inputService->IsDown(DIK_UP) || inputService->IsDown(DIK_DOWN))
			offset.y =  (inputService->IsDown(DIK_DOWN)?-1:1) * elapsedTime * 0.5 * station->Get_MoveRate();
		if(abs(offset.x) > std::numeric_limits<double>::epsilon() || abs(offset.y) > std::numeric_limits<double>::epsilon())
			this->Move(offset);

		//Controls for the station if its an other type
		if(inputService->IsDown(DIK_A) || inputService->IsDown(DIK_D))
			this->Rotate((inputService->IsDown(DIK_A)?1:-1) * elapsedTime * 0.5 * station->Get_RotateRate(), type);

	}

	//Controls for the station if its a turret type
	if(type == StationType::Turret)
	{
		//Move the target
		if(inputService->IsDown(DIK_RIGHT) || inputService->IsDown(DIK_LEFT))
			offset.x = (inputService->IsDown(DIK_LEFT)?-1:1) * elapsedTime * 0.5 * this->m_pTarget->Get_MoveRate();
		if(inputService->IsDown(DIK_UP) || inputService->IsDown(DIK_DOWN))
			offset.y =  (inputService->IsDown(DIK_DOWN)?1:-1) * elapsedTime * 0.5 * this->m_pTarget->Get_MoveRate();
		if(abs(offset.x) > std::numeric_limits<double>::epsilon() || abs(offset.y) > std::numeric_limits<double>::epsilon())
			this->MoveTarget(offset);

		if(inputService->IsDown(DIK_SPACE))
			this->Fire();

		////Controls for the station if its an other type
		//if(inputService->IsDown(DIK_A) || inputService->IsDown(DIK_D))
		//	this->Rotate((inputService->IsDown(DIK_A)?1:-1) * elapsedTime * 0.5 * station->Get_RotateRate(), type);
	}

	if(type == StationType::Shield)
	{
		//Controls for the station if its an other type
		if(inputService->IsDown(DIK_A) || inputService->IsDown(DIK_D))
			this->Rotate((inputService->IsDown(DIK_A)?1:-1) * elapsedTime * 0.5 * station->Get_RotateRate(), type);
	}
}

//All XBox control input is handled here
void CPlayerCrewController::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	float elapsedTime = gameTime->Get_ElapsedTime();
	float buttonTimer = 0.0f;
	bool toPilot = false;

	//Get the bound unit and its station
	CCrew* boundCrew = this->Get_BoundUnit();
	CStation* station = boundCrew->Get_BoundStation();
	

	//if the station is null, by default, assign it to the pilot station
	/**
	 * Note by Eric Moreau: 
	 * will change in later versions to handle assigning a station to
	 * all crew members at the beginning of the game. Needs to check station occupancy
	 */
	if(station == NULL)
	{
		this->SwitchStation(0);

		this->CreateTarget();
		return;
	}

	//Get the station's type
	StationType type = station->Get_StationType();

	//move the crew member to the pilot station. Detects 
	//if(inputService->IsPressed(XButtonState::GAMEPAD_LEFT_SHOULDER) && inputService->IsPressed(XButtonState::GAMEPAD_RIGHT_SHOULDER))
	if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_DOWN))
	{
		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		this->SwitchStation(0);

		this->m_pTarget->Set_IsTurretBound(false);

		//unbind the target from its turret
		if(this->m_iStationIndex > 0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();

		return;
	}	

	//Switch the station counter clockwise
	if(inputService->IsPressed(XButtonState::GAMEPAD_LEFT_SHOULDER))
	{
		// play the left sound
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);

		//unbind the current target
		if(this->m_iStationIndex >0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();

		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		//adjust the index
		this->m_iStationIndex += 1;
		if(this->m_iStationIndex > 6)
			this->m_iStationIndex = 1;

		//Call switch station with an adjusted index passed in
		this->SwitchStation(this->m_iStationIndex);

		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Turret)
		{
			//Sets the target's draw flag to true if the station type is a target
			this->m_pTarget->Set_IsTurretBound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->BindTarget(this->m_pTarget);
		}else{
			//Sets the target's draw flag to false if the station type is not a turret
			this->m_pTarget->Set_IsTurretBound(false);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();
		}
		return;
	}
	//Switch the station clockwise
	else if(inputService->IsPressed(XButtonState::GAMEPAD_RIGHT_SHOULDER))
	{
		// play the left sound
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);

		//unbind the current target
		if(this->m_iStationIndex > 0)
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();

		//if the current station is a shield station, we need to disable the shield
		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Shield)
		{
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(false);
		}

		//adjust the index
		this->m_iStationIndex -= 1;
		if(this->m_iStationIndex < 1)
			this->m_iStationIndex = 6;

		//Call switch station with an adjusted index passed in
		this->SwitchStation(this->m_iStationIndex);

		if(this->Get_BoundUnit()->Get_Ship()->GetStationAt(this->m_iStationIndex)->Get_StationType() == StationType::Turret)
		{
			//Sets the target's draw flag to true if the station type is a target
			this->m_pTarget->Set_IsTurretBound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->BindTarget(this->m_pTarget);
		}else{
			//Sets the target's draw flag to false if the station type is not a turret
			this->m_pTarget->Set_IsTurretBound(false);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->Get_Shield()->Set_Bound(true);
			this->Get_BoundUnit()->Get_Ship()->GetTurretAt(this->m_iStationIndex)->UnbindTarget();
		}
		return;
	}

	D3DXVECTOR2 offset;
	offset.x = 0;
	offset.y = 0;

	//Controls for the station if its a pilot type
	if(type == StationType::Pilot)
	{
		//Move the station if the threshhold of the joystick is past a certain point
		if(abs(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X)) > 0.1f || abs(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_Y)) > 0.1f)
		{
			D3DXVECTOR2 offset;
			//offset.x = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X);
			//offset.y = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_Y);

			//// THIS IS ALL I FREAKING WANTED TO KNOW!
			//// GOOD GREIF!
			//float length = D3DXVec2Length(&offset);
			//CShip::engineNoise->SetPitch(XACTPITCH_MIN + XACTPITCH_MAX_TOTAL*length);
			//CShip::engineNoise->SetVolume(length);

			offset.x = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X) * elapsedTime * -station->Get_MoveRate();
			offset.y = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_Y) * elapsedTime * station->Get_MoveRate();
			this->Move(offset);
		}

		//Grab a rotation value
		float rotateValue = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_TRIGGER) * elapsedTime;
		rotateValue -= inputService->GetPosition(XAnalogState::GAMEPAD_RIGHT_TRIGGER) * elapsedTime;
	
		//Rotate the bound unit
		if(abs(rotateValue) > std::numeric_limits<double>::epsilon())
			this->Rotate(rotateValue * station->Get_RotateRate(), type);
	}

	//Controls for the station if its a turret type
	if(type == StationType::Turret)
	{
		//Move the station if the threshhold of the joystick is past a certain point
		if(abs(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X)) > 0.1f || abs(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_Y)) > 0.1f)
		{
			D3DXVECTOR2 offset;
			offset.x = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X) * elapsedTime * this->m_pTarget->Get_MoveRate();
			offset.y = inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_Y) * elapsedTime * -this->m_pTarget->Get_MoveRate();
			this->MoveTarget(offset);
		}

		//Call the fire event
		if(inputService->GetPosition(XAnalogState::GAMEPAD_RIGHT_TRIGGER) > 0.1f)
			this->Fire();
	}
	
	//Controls for the shield station
	if(type == StationType::Shield)
	{
		if (abs(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X)) > 0.1)
			this->Rotate(elapsedTime * -(inputService->GetPosition(XAnalogState::GAMEPAD_LEFT_THUMB_X)) * station->Get_RotateRate(), type);

		if(inputService->IsDown(XButtonState::GAMEPAD_X))
			this->Set_Color(3);
		else if(inputService->IsDown(XButtonState::GAMEPAD_Y))
			this->Set_Color(2);
		else if(inputService->IsDown(XButtonState::GAMEPAD_A))
			this->Set_Color(1);
		else if(inputService->IsDown(XButtonState::GAMEPAD_B))
			this->Set_Color(0);
		else 
			this->Set_Color(5);
	}
}


//Method to create the target reticule associated with this player object
void CPlayerCrewController::CreateTarget()
{
	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	
	::D3DXMatrixTranslation(&translation, 5.0f, 0.0f, 0.0f);
	::D3DXMatrixIdentity(&rotation);

	this->m_pTarget = new CTarget(
		this->Get_Game(),  
		translation, 
		rotation,
		this->Get_BoundUnit()->Get_Ship()->Get_Transform());

	this->Get_Game()->Get_Components()->Add(this->m_pTarget);
}

void CPlayerCrewController::MoveTarget(D3DXVECTOR2 offset)
{
	this->m_pTarget->Move(offset);
}
#pragma endregion

#pragma region Overriden Methods
void CPlayerCrewController::SwitchStation(int index)
{
	if(this->Get_BoundUnit() != NULL && this->Get_BoundUnit()->Get_BoundStation() != NULL)
		this->Get_BoundUnit()->Get_BoundStation()->Set_HUDEnabled(false);
	
	CBaseCrewController::SwitchStation(index);

	if(this->Get_BoundUnit() != NULL && this->Get_BoundUnit()->Get_BoundStation() != NULL)
		this->Get_BoundUnit()->Get_BoundStation()->Set_HUDEnabled(true);
}

//Unbind Controllers
void CPlayerCrewController::Unbind()
{
	if(this->Get_TurretController()->Get_IsBound())
		this->Get_TurretController()->Unbind();

	if(this->Get_ShipController()->Get_IsBound())
		this->Get_ShipController()->Unbind();
}

//Initialize the controller
void CPlayerCrewController::Initialize()
{
	//Call CGameComponent's initialize
	CBaseCrewController::Initialize();

	//Grab the input manager's input service
	IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
	this->m_pInputService = inputManager->GetInput(PlayerIndex::One);
	if(this->m_pInputService == NULL)
		exit(-1);

	if(this->m_pTarget != NULL)
		this->m_pTarget->Initialize();
}

//Update the controller every frame
void CPlayerCrewController::Update(CGameTime* gameTime)
{
	//Call CGameComponent's update
	CGameComponent::Update(gameTime);

	//Check if the bound unit is null
	if(this->Get_BoundUnit() == NULL)
		return;
	
	//Flag the input service based on platform
	if(this->m_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
	else
		this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);

	this->NetworkUpdate(gameTime, this->Get_BoundUnit()->Get_Identifier());

}
#pragma endregion