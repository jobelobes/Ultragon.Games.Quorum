#include "Quorum.h"

class ControlsScreen : public CGameScreen
{
	private:
		bool m_bIsEnabled;
		std::vector<CSprite2D> controllerSprites;
		std::vector<CSprite2D> sprites;
		int m_iSelectedEntry;
		std::vector<char*> m_sMenuEntries;
		CSpriteBatch* m_pSpriteBatch;
		IInputService* m_pInputService;
		
	public:
		#pragma region Constructors and Finalizers
		ControlsScreen(CGame*);
		~ControlsScreen();
		#pragma endregion

		#pragma region Overridden Methods
		void Initialize();
		void LoadContent();
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		void HandleXboxInput(IXBoxInputService*, CGameTime*);
		void Update(CGameTime*);
		void Draw(CGameTime*);
		void Activate(CGameScreen*);
		void Deactivate();

		virtual char* Get_Type();
		#pragma endregion
};