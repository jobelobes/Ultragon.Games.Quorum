#include "Quorum.h"

class TestScreen : public CGameScreen
{
	private:
		#pragma region Variables
		IInputService* m_pInputService;
		#pragma endregion

		#pragma region Methods
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		void HandleXboxInput(IXBoxInputService*, CGameTime*);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		TestScreen(CGame*);
		~TestScreen();
		#pragma endregion

		#pragma region Overriden Methods
		void Initialize();
		void Update(CGameTime*);
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods
		void Activate(CGameScreen*);
		void Deactivate();
		#pragma endregion
};