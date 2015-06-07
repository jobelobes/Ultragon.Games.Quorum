/**
 * @description 
 *   Defines the Credits screen for the game. Credits screen gives credit to the 
 *	 developers of the game. Shown before the title screen is shown.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class CreditsScreen : public CGameScreen
{
	private:
		bool m_bIsEnabled;
		std::vector<CSprite2D> sprites;
		int m_iSelectedEntry;
		std::vector<char*> m_sMenuEntries;
		CSpriteBatch* m_pSpriteBatch;
		IInputService* m_pInputService;
		
	public:
		CreditsScreen(CGame*);
		~CreditsScreen();

		void Initialize();
		void LoadContent();
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		void HandleXboxInput(IXBoxInputService*, CGameTime*);
		void Update(CGameTime*);
		void Draw(CGameTime*);
		void Activate(CGameScreen*);
		void Deactivate();

		#pragma region Overridden Methods
		virtual char* Get_Type();
		#pragma endregion
};