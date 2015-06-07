/**
 * @description 
 *   Defines the End Game screen for the game. The End Game screen show whether
 *   you won or lost the game.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class EndGameScreen : public CGameScreen
{
	private:
		bool m_bIsEnabled;
		std::vector<CSprite2D> sprites;
		int m_iSelectedEntry;
		std::vector<char*> m_sMenuEntries;
		CSpriteBatch* m_pSpriteBatch;
		IInputService* m_pInputService;
		char* winMsg;

		double m_dTimeToWin;
		int m_iNumEnemiesKilled;
		bool m_bHasScored;
		double m_dScore;

		void CalculateScore();
		
	public:
		EndGameScreen(CGame*);
		~EndGameScreen();

		void SetTimeToWin(double);
		void SetNumEnemiesKilled(int);

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