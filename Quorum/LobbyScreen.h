/**
 * @description 
 *   Defines the Lobby screen for the game. The Lobby screen is where people can challenge
 *   other people to play in networked games as well as chat.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class LobbyScreen : public CGameScreen
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

		int m_iSelectedGameIdentifier;
		int m_iSelectedGame;
		int m_iGameCount;
		RECT m_sGamesRect;
		char m_sGamesBuffer[1024];
		#pragma endregion

		#pragma region Methods
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);
		void HandleXboxInput(IXBoxInputService*, CGameTime*);

		void CreateGame();
		void JoinGame(D3DX_SESSION_DESCRIPTION);
		#pragma endregion
		
	public:
		#pragma region Constructors and Finalizers
		LobbyScreen(CGame*);
		~LobbyScreen();
		#pragma endregion

		#pragma region Overridden Methods
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Overriden Methods
		void LoadContent();
		void Update(CGameTime*);
		void Draw(CGameTime*);

		void Activate(CGameScreen*);
		void Deactivate();
		#pragma endregion
};