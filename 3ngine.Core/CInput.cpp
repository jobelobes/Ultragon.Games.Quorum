#include "3ngine.Core.h"

CInput::CInput(CGameWindow* cWindow)
//CInput::CInput(HINSTANCE &hInstance, HWND &hwnd)
{
	cGameWindow = NULL;
	m_pDI = NULL;
	m_pKeyboardDevice = NULL;
	m_pMouseDevice = NULL;
	g_Controllers = new CONTROLLER_STATE[MAX_CONTROLLERS];
	ZeroMemory(g_Controllers, sizeof(CONTROLLER_STATE) * MAX_CONTROLLERS);
	ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));

	cGameWindow = cWindow;
	InitDirectInput();
	InitKeyboard();
	InitMouse();
	/*hInst = hInstance;
	hWnd = hwnd;
	InitDirectInput(hInstance);
	InitKeyboard(hwnd);
	InitMouse(hwnd);*/
}

CInput::~CInput()
{
	//Release all devices if not released already
	if(m_pKeyboardDevice)
	{
		m_pKeyboardDevice->Unacquire();
		m_pKeyboardDevice->Release();
		m_pKeyboardDevice = NULL;
	}

	if(m_pMouseDevice)
	{
		m_pMouseDevice->Unacquire();
		m_pMouseDevice->Release();
		m_pMouseDevice = NULL;
	}

	if(m_pDI)
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}

//Try to create DirectInput device
HRESULT CInput::InitDirectInput()
//HRESULT CInput::InitDirectInput(HINSTANCE &hInstance)
{	
	if(FAILED(DirectInput8Create(cGameWindow->Get_Instance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	//if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	{
		return S_FALSE;
	}

	return S_OK;
}

//Try to create DirectInput Keyboard and set up parameters
HRESULT CInput::InitKeyboard()
//HRESULT CInput::InitKeyboard(HWND &hWnd)
{
	if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL)))
		return S_FALSE;
	if(FAILED(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
		return S_FALSE;
	if(FAILED(m_pKeyboardDevice->SetCooperativeLevel(cGameWindow->Get_Handle(), DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
		return S_FALSE;
	if(FAILED(m_pKeyboardDevice->Acquire()))
		return S_FALSE;

	return S_OK;
}

//Try to create DirectInput Mouse and set up parameters
HRESULT CInput::InitMouse()
//HRESULT CInput::InitMouse(HWND &hWnd)
{	
	if(FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
		return S_FALSE;
	if(FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse)))
		return S_FALSE;
	if(FAILED(m_pMouseDevice->SetCooperativeLevel(cGameWindow->Get_Handle(), DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
		return S_FALSE;
	if(FAILED(m_pMouseDevice->Acquire()))
		return S_FALSE;

	return S_OK;
}

//Poll Mouse for MouseState
HRESULT CInput::UpdateMouseState()
{
	HRESULT r = m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	if(FAILED(r))
	{
		//Potential loss of Mouse. Try to get the device back if input was lost.
		if(r == DIERR_INPUTLOST)
		{
			// Mouse lost, zero out mouse data structure.
			ZeroMemory(&m_MouseState, sizeof(m_MouseState));

			while(r == DIERR_INPUTLOST)
			{
				r = m_pMouseDevice->Acquire();
				if(SUCCEEDED(r))
				{
					m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (void**)&m_MouseState);
				}
				else
				{
					return S_FALSE;
				}
			}
		}
		else if (r == DIERR_NOTACQUIRED)
		{
			while(r == DIERR_NOTACQUIRED)
			{
				r = m_pMouseDevice->Acquire();
				if(SUCCEEDED(r))
				{
					m_pMouseDevice->GetDeviceState(sizeof(m_MouseState), (void**)&m_MouseState);
				}
				else
				{
					return S_FALSE;
				}
			}
		}
		else
		{
			return S_FALSE;
		}		
	}
	else
	{
		return S_OK;
	}
}

HRESULT CInput::Update()
{
	if(FAILED(UpdateMouseState()))
		return S_FALSE;
	if(FAILED(UpdateControllerState()))
		return S_FALSE;
	if(FAILED(UpdateKeyboardState()))
		return S_FALSE;
}

HRESULT CInput::UpdateKeyboardState()
{
	HRESULT r = m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), (void**)&m_KeyBuffer);
	
	if(FAILED(r))
	{
		//Potential loss of Keyboard. Can try to get the device back if input was lost.
		if(r == DIERR_INPUTLOST)
		{
			// Keyboard lost, zero out keyboard data structure.
			ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));

			while(r == DIERR_INPUTLOST)
			{
				r = m_pKeyboardDevice->Acquire();
				if(SUCCEEDED(r))
				{
					m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), (void**)&m_KeyBuffer);
				}
				else
				{
					return S_FALSE;
				}
			}
		}
		else if (r == DIERR_NOTACQUIRED)
		{
			while(r == DIERR_NOTACQUIRED)
			{
				r = m_pKeyboardDevice->Acquire();
				if(SUCCEEDED(r))
				{
					m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), (void**)&m_KeyBuffer);
				}
				else
				{
					return S_FALSE;
				}
			}
		}
		else
		{
			return S_FALSE;
		}		
	}
	else
	{
		return S_OK;
	}
}

HRESULT CInput::UpdateControllerState()
{
    DWORD dwResult;
    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ )
    {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &g_Controllers[i].state);

        if(dwResult == ERROR_SUCCESS)
            g_Controllers[i].bConnected = true;
        else
            g_Controllers[i].bConnected = false;
    }

    return S_OK;
}

CONTROLLER_STATE CInput::GetControllerState(int playerIndex)
{
	DWORD pIndex = (DWORD)playerIndex;
	return g_Controllers[playerIndex];
}

bool CInput::IsXButtonPressed(int playerIndex, XButtonState xButtonState)
{
	if(g_Controllers[playerIndex].state.Gamepad.wButtons & xButtonState)
		return true;
	else
		return false;
}

int CInput::GetAnalogPosition(int playerIndex, XAnalogState xAnalogState)
{
	float analogValue = 0;

	if(xAnalogState == GAMEPAD_LEFT_TRIGGER)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.bLeftTrigger;
		analogValue = analogValue / 255.0f;
	}
	else if(xAnalogState == GAMEPAD_RIGHT_TRIGGER)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.bRightTrigger;
		analogValue = analogValue / 255.0f;
	}
	else if(xAnalogState == GAMEPAD_LEFT_THUMB_X)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.sThumbLX;
		analogValue = analogValue / 32767.0f;
	}
	else if(xAnalogState == GAMEPAD_LEFT_THUMB_Y)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.sThumbLY;
		analogValue = analogValue / 32767.0f;
	}
	else if(xAnalogState == GAMEPAD_RIGHT_THUMB_X)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.sThumbRX;
		analogValue = analogValue / 32767.0f;
	}
	else if(xAnalogState == GAMEPAD_RIGHT_THUMB_Y)
	{
		analogValue = (float)g_Controllers[playerIndex].state.Gamepad.sThumbRY;
		analogValue = analogValue / 32767.0f;
	}

	if(!(xAnalogState == GAMEPAD_LEFT_TRIGGER || xAnalogState == GAMEPAD_RIGHT_TRIGGER))
	{
		//0.24f = INPUT_DEADZONE nomalized
		if(analogValue < 0.24f && analogValue > -0.24f)
			analogValue = 0;
	}

	return analogValue;
}

void CInput::StartRumble(int playerIndex, float leftRumble, float rightRumble)
{
	g_Controllers[playerIndex].vibration.wLeftMotorSpeed = (WORD)(leftRumble * 65535.0f);
	g_Controllers[playerIndex].vibration.wRightMotorSpeed = (WORD)(rightRumble * 65535.0f);

	XInputSetState(playerIndex, &g_Controllers[playerIndex].vibration);
}

void CInput::StopRumble(int playerIndex)
{
	g_Controllers[playerIndex].vibration.wLeftMotorSpeed = 0;
	g_Controllers[playerIndex].vibration.wRightMotorSpeed = 0;

	XInputSetState(playerIndex, &g_Controllers[playerIndex].vibration);
}

//Test if key is pressed
bool CInput::IsKeyPressed(char key)
{
	//HRESULT r = m_pKeyboardDevice->GetDeviceState(sizeof(m_KeyBuffer), &m_KeyBuffer);
	//
	//if(FAILED(r))
	//{
	//	//Potential loss of Keyboard. Can try to get the device back if input was lost.
	//	if(r == DIERR_INPUTLOST)
	//	{
	//		if(FAILED(m_pKeyboardDevice->Acquire()))
	//		{
	//			return S_FALSE;
	//		}
	//		return S_OK;
	//	}

	//	return false;
	//}

	//Tests if key is pressed
	if(m_KeyBuffer[key] & 0x80)
		return true;

	return false;
}

//Tests if a mouse button is down
bool CInput::IsMouseButtonDown(int button)
{
	if(m_MouseState.rgbButtons[button] & 0x80)
		return true;

	return false;
}

//Get Mouse's change in X
float CInput::MouseDX()
{
	return (float)m_MouseState.lX;
}

//Get Mouse's change in Y
float CInput::MouseDY()
{
	return (float)m_MouseState.lY;
}

//Get Mouse's change in Z
float CInput::MouseDZ()
{	
	return (float)m_MouseState.lZ;
}