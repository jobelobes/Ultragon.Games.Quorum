/**
 * @description 
 *   Defines the Title screen for the game. Controls what is drawn on the title screen
 *   and what menu options are available from the title screen and which screens can be 
 *   navigated to.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class TitleScreen : public CGameScreen
{
	private:
		#pragma region Variables
		bool m_bIsEnabled;
		std::vector<CSprite2D> sprites;
		int m_iSelectedEntry;
		std::vector<char*> m_sMenuEntries;
		CSpriteBatch* m_pSpriteBatch;
		CAnimation* m_pAnimation;
		#pragma endregion

	protected:
		#pragma region Methods
		/**
		 * Derived HandleWindowsInput method from the CGameScreen.
		 *
		 * @param	windowsInputService		input service to be used to handle user input
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void HandleWindowsInput(IWindowsInputService* windowsInputService, CGameTime* gameTime);

		/**
		 * Derived HandleXboxInput method from the CGameScreen.
		 *
		 * @param	xBoxInputService	input service to be used to handle user input
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void HandleXboxInput(IXBoxInputService* xBoxInputService, CGameTime* gameTime);
		#pragma endregion

	public:
		#pragma region Overridden Properties
		/**
		 * Derived Get_Type method from the CGameScreen.
		 */
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * TitleScreen Constructor
		 *
		 * @param	game	current game
		 */
		TitleScreen(CGame* game);

		/**
		 * TitleScreen Destructor
		 */
		~TitleScreen();
		#pragma endregion

		#pragma region Methods
		/**
		 * Derived Activate method from the CGameScreen.
		 *
		 * @param	parentScreen	screen that this screen's activate is called from
		 */
		void Activate(CGameScreen* parentScreen);

		/**
		 * Derived Deactivate method from the CGameScreen.
		 */
		void Deactivate();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived LoadContent method from the CDrawableGameComponent.
		 */
		void LoadContent();

		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		void Update(CGameTime* gameTime);

		/**
		 * Derived Draw method from the CDrawableGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		void Draw(CGameTime* gameTime);
		#pragma endregion
};