#include "3ngine.Core.h"

#pragma region Static Variables
const int CXBoxInput::MaxControllers = 4;
const float CXBoxInput::DeadZone = (0.24f * FLOAT(0x7FFF));
#pragma endregion

#pragma region Properties
Platform CXBoxInput::Get_Platform()
{
	return Platform::Xbox360;
}

PlayerIndex CXBoxInput::Get_PlayerIndex()
{
	return this->m_ePlayerIndex;
}

bool CXBoxInput::Get_IsConnected()
{
	return this->m_pCurState->bConnected;
}
#pragma endregion

#pragma region Constructors and Finalizers
CXBoxInput::CXBoxInput(CGame* game)
: CGameComponent(game)
{
	this->m_ePlayerIndex = PlayerIndex::One;

	this->m_pCurState = new CONTROLLER_STATE();
	this->m_pPrevState = new CONTROLLER_STATE();

	game->Get_Services()->AddService(this);
}

CXBoxInput::CXBoxInput(CGame* game, PlayerIndex index)
: CGameComponent(game)
{
	this->m_ePlayerIndex = index;

	this->m_pCurState = new CONTROLLER_STATE();
	this->m_pPrevState = new CONTROLLER_STATE();

	game->Get_Services()->AddService(this);
}

CXBoxInput::~CXBoxInput()
{

}
#pragma endregion

#pragma region Methods
void CXBoxInput::StartRumble(float left, float right)
{
	this->m_pCurState->vibration.wLeftMotorSpeed = (WORD)(left * 65535.0f);
	this->m_pCurState->vibration.wRightMotorSpeed = (WORD)(right * 65535.0f);

	XInputSetState((int)this->m_ePlayerIndex, &this->m_pCurState->vibration);
}

void CXBoxInput::StopRumble()
{
	this->m_pCurState->vibration.wLeftMotorSpeed = 0;
	this->m_pCurState->vibration.wRightMotorSpeed = 0;

	XInputSetState((int)this->m_ePlayerIndex, &this->m_pCurState->vibration);
}
#pragma endregion

#pragma region Overriden Methods
bool CXBoxInput::IsDown(XButtonState button)
{
	return (this->m_pCurState->state.Gamepad.wButtons & button);
}
bool CXBoxInput::IsUp(XButtonState button)
{
	return !(this->m_pCurState->state.Gamepad.wButtons & button);
}

bool CXBoxInput::IsPressed(XButtonState button)
{
	return (this->m_pPrevState->state.Gamepad.wButtons & button) && !(this->m_pCurState->state.Gamepad.wButtons & button);
}

float CXBoxInput::GetPosition(XAnalogState analog)
{
	float analogValue = 0;

	if(analog == GAMEPAD_LEFT_TRIGGER)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.bLeftTrigger;
		analogValue = analogValue / 255.0f;
	}
	else if(analog == GAMEPAD_RIGHT_TRIGGER)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.bRightTrigger;
		analogValue = analogValue / 255.0f;
	}
	else if(analog == GAMEPAD_LEFT_THUMB_X)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.sThumbLX;
		analogValue = analogValue / 32767.0f;
	}
	else if(analog == GAMEPAD_LEFT_THUMB_Y)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.sThumbLY;
		analogValue = analogValue / 32767.0f;
	}
	else if(analog == GAMEPAD_RIGHT_THUMB_X)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.sThumbRX;
		analogValue = analogValue / 32767.0f;
	}
	else if(analog == GAMEPAD_RIGHT_THUMB_Y)
	{
		analogValue = (float)this->m_pCurState->state.Gamepad.sThumbRY;
		analogValue = analogValue / 32767.0f;
	}

	if(!(analog == GAMEPAD_LEFT_TRIGGER || analog == GAMEPAD_RIGHT_TRIGGER))
	{
		//0.24f = INPUT_DEADZONE nomalized
		if(analogValue < 0.24f && analogValue > -0.24f)
			analogValue = 0;
	}

	return analogValue;
}

void CXBoxInput::Initialize()
{
	CGameComponent::Initialize();
}

void CXBoxInput::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	CONTROLLER_STATE* tmp = this->m_pPrevState;
	this->m_pPrevState = this->m_pCurState;
	this->m_pCurState = tmp;

    // Simply get the state of the controller from XInput.
	DWORD dwResult = ::XInputGetState((int)this->m_ePlayerIndex, &this->m_pCurState->state);
    this->m_pCurState->bConnected = (dwResult == ERROR_SUCCESS);
}
#pragma endregion

#pragma region Static Methods
bool CXBoxInput::IsControllerConnected(PlayerIndex index)
{
	XINPUT_STATE state;
	DWORD dwResult = ::XInputGetState((int)index, &state);
    return (dwResult == ERROR_SUCCESS);
}
#pragma endregion