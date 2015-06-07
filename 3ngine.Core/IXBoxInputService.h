#include "3ngine.Core.h"

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

class CORE_API IXBoxInputService : public IInputService
{
	public:
		#pragma region Properties
		virtual Platform Get_Platform() = 0;
        virtual PlayerIndex Get_PlayerIndex() = 0;
		#pragma endregion

		#pragma region Methods
		virtual bool IsDown(XButtonState) = 0;
		virtual bool IsUp(XButtonState) = 0; 
		virtual bool IsPressed(XButtonState) = 0;
		virtual float GetPosition(XAnalogState xAnalogState) = 0;
		#pragma endregion
};