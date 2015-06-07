#include "3ngine.Core.h"

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE (0.24f * FLOAT(0x7FFF))  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
struct CONTROLLER_STATE
{
    XINPUT_STATE state;
    bool bConnected;
	XINPUT_VIBRATION vibration;
};

enum XButtonState
{
	GAMEPAD_DPAD_UP = 0x00000001,
	GAMEPAD_DPAD_DOWN = 0x00000002,
	GAMEPAD_DPAD_LEFT = 0x00000004,
	GAMEPAD_DPAD_RIGHT = 0x00000008,
	GAMEPAD_START = 0x00000010,
	GAMEPAD_BACK = 0x00000020,
	GAMEPAD_LEFT_THUMB = 0x00000040,
	GAMEPAD_RIGHT_THUMB = 0x00000080,
	GAMEPAD_LEFT_SHOULDER = 0x0100,
	GAMEPAD_RIGHT_SHOULDER = 0x0200,
	GAMEPAD_A = 0x1000,
	GAMEPAD_B = 0x2000,
	GAMEPAD_X = 0x4000,
	GAMEPAD_Y = 0x8000
};

enum XAnalogState
{
	GAMEPAD_LEFT_TRIGGER = 0,
	GAMEPAD_RIGHT_TRIGGER = 1,
	GAMEPAD_LEFT_THUMB_X = 2,
	GAMEPAD_LEFT_THUMB_Y = 3,
	GAMEPAD_RIGHT_THUMB_X = 4,
	GAMEPAD_RIGHT_THUMB_Y = 5
};

class CORE_API CInput
{
public:
	CInput(CGameWindow* cWindow);
	//CInput(HINSTANCE &hInstance, HWND &hwnd);
	~CInput();
	/*HRESULT InitDirectInput(HINSTANCE &hInstance);
	HRESULT InitKeyboard(HWND &hWnd);
	HRESULT InitMouse(HWND &hWnd);*/
	HRESULT InitDirectInput();
	HRESULT InitKeyboard();
	HRESULT InitMouse();
	HRESULT UpdateMouseState();
	HRESULT UpdateKeyboardState();
	HRESULT Update();
	HRESULT UpdateControllerState();
	CONTROLLER_STATE GetControllerState(int playerIndex);
	bool IsXButtonPressed(int playerIndex, XButtonState xButtonState);
	int GetAnalogPosition(int playerIndex, XAnalogState xAnalogState);
	void StartRumble(int playerIndex, float leftRumble, float rightRumble);
	void StopRumble(int playerIndex);
	bool IsKeyPressed(char key);
	bool IsMouseButtonDown(int button);
	float MouseDX();
	float MouseDY();
	float MouseDZ();

private:
	CGameWindow* cGameWindow;
	HINSTANCE hInst;
	HWND hWnd;
	LPDIRECTINPUT8 m_pDI;
	LPDIRECTINPUTDEVICE8 m_pKeyboardDevice;
	char m_KeyBuffer[256];
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;
	DIMOUSESTATE2 m_MouseState;
	CONTROLLER_STATE* g_Controllers;
};