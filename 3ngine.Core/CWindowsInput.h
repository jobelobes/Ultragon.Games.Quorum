#include "3ngine.Core.h"

class CORE_API CWindowsInput : public CGameComponent, public IWindowsInputService
{
	private:
		#pragma region Variables
		char m_pCurKeyBuffer[256];
		char m_pPrevKeyBuffer[256];

		CGameWindow* m_pWindow;
		PlayerIndex m_ePlayerIndex;
		
		LPPOINT m_pMousePos;
		DIMOUSESTATE2 m_sMouseState;
		LPDIRECTINPUT8 m_pDirectInputContext;
		LPDIRECTINPUTDEVICE8 m_pKeyboardDevice;
		LPDIRECTINPUTDEVICE8 m_pMouseDevice;
		#pragma endregion
		
		#pragma region Methods
		void InitializeKeyboard();
		void InitializeMouse();
		void UpdateMouse();
		void UpdateKeyboard();
		#pragma endregion

	public:
		#pragma region Properties
		virtual Platform Get_Platform();
        virtual PlayerIndex Get_PlayerIndex();
		virtual DIMOUSESTATE2 Get_MouseState();
		LPPOINT Get_MousePosition();
		#pragma endregion

		#pragma region Constructors and Finalizers
		CWindowsInput(CGame*);
		CWindowsInput(CGame*, PlayerIndex);
		~CWindowsInput();
		#pragma endregion

		#pragma region IWindowsInputService Methods
		virtual bool IsDown(int key);
		virtual bool IsUp(int key); 
		virtual bool IsPressed(int key);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion
};