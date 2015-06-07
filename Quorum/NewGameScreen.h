/**
 * @description 
 *   Defines the New Game screen for the game. The New Game screen allows players
 *   choose which type of game they would like to play.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class NewGameScreen : public CGameScreen
{
	private:
		#pragma region Static Variables
		static INetworkService* g_pNetwork;
		#pragma endregion

		#pragma region Variables
		std::vector<CSprite2D> sprites;
		std::vector<char*> m_sMenuEntries;
		CSpriteBatch* m_pSpriteBatch;

		RECT m_sClientsRect;
		char m_sClientBuffer[1024];
		#pragma endregion
		
	public:
		#pragma region Constructors and Finalizers
		NewGameScreen(CGame*);
		~NewGameScreen();
		#pragma endregion

		#pragma region Overriden Methods
		void Activate(CGameScreen*);
		void Deactivate();

		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		void HandleXboxInput(IXBoxInputService*, CGameTime*);

		void Initialize();
		void LoadContent();
		void Update(CGameTime*);
		void Draw(CGameTime*);

		virtual char* Get_Type();
		#pragma endregion
};