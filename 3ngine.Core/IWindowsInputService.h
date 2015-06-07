#include "3ngine.Core.h"

class CORE_API IWindowsInputService : public IInputService
{
	public:
		#pragma region Properties
		virtual Platform Get_Platform() = 0;
        virtual PlayerIndex Get_PlayerIndex() = 0;
		virtual DIMOUSESTATE2 Get_MouseState() = 0;
		virtual LPPOINT Get_MousePosition() = 0;
		#pragma endregion

		#pragma region Methods
		virtual bool IsDown(int) = 0;
		virtual bool IsUp(int) = 0; 
		virtual bool IsPressed(int) = 0;
		#pragma endregion
};