#include "3ngine.Core.h"


class CORE_API CXBoxInput : public CGameComponent, public IXBoxInputService
{
	private:
		#pragma region Nested Classes
		struct CONTROLLER_STATE
		{
			XINPUT_STATE state;
			bool bConnected;
			XINPUT_VIBRATION vibration;
		};
		#pragma endregion

		#pragma region Static Variables
		static const int MaxControllers;
		static const float DeadZone;
		#pragma endregion

		#pragma region Variables
		CGameWindow* m_pWindow;
		PlayerIndex m_ePlayerIndex;

		CONTROLLER_STATE* m_pCurState;
		CONTROLLER_STATE* m_pPrevState;
		#pragma endregion

		#pragma region Methods
		HRESULT UpdateControllerState();
		#pragma endregion

	public:
		#pragma region IXBoxInputService Properties
		virtual Platform Get_Platform();
        virtual PlayerIndex Get_PlayerIndex();
		#pragma endregion

		#pragma region Properties
		bool Get_IsConnected();
		#pragma endregion

		#pragma region Constructors and Finalizers
		CXBoxInput(CGame*);
		CXBoxInput(CGame*, PlayerIndex);
		~CXBoxInput();
		#pragma endregion

		#pragma region Methods
		void StartRumble(float, float);
		void StopRumble();
		#pragma endregion

		#pragma region IXBoxInputService Methods
		virtual bool IsDown(XButtonState);
		virtual bool IsUp(XButtonState); 
		virtual bool IsPressed(XButtonState);
		virtual float GetPosition(XAnalogState);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion

		#pragma region Static Methods
		static bool IsControllerConnected(PlayerIndex);
		#pragma endregion
};
