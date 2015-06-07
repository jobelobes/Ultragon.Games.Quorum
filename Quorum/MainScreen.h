/**
 * @description 
 *   Defines the Main screen for the game. Screen that is used to draw all gameplay content.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

class MainScreen : public CGameScreen
{
	private:
		#pragma region Variables
		bool m_bActivated;
		time_t startTime;
		
		IInputService* m_pInputService;
		CWorldBackground* m_pBackground;
		CQuorumMissionManager* m_pMissionManager;
		CAnimation* m_pArrowGuide;
		D3DXMATRIX m_pArrowTrans;
		#pragma endregion

		#pragma region Methods

		/**
		 * Derived HandleWindowsInput method from the CGameScreen.
		 *
		 * @param	windowsInputService		input service to be used to handle user input
		 * @param	gameTime	the games elapsed/total time
		 */
		void HandleWindowsInput(IWindowsInputService*, CGameTime*);

		/**
		 * Derived HandleXboxInput method from the CGameScreen.
		 *
		 * @param	xBoxInputService	input service to be used to handle user input
		 * @param	gameTime	the games elapsed/total time
		 */
		void HandleXboxInput(IXBoxInputService*, CGameTime*);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers

		/**
		 * MainScreen Constructor
		 *
		 * @param	game	current game
		 */
		MainScreen(CGame* game);

		/**
		 * MainScreen Destructor
		 */
		~MainScreen();
		#pragma endregion

		#pragma region Overriden Methods

		/**
		 * Derived Intialize method from the CGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived LoadContent method from the CDrawableGameComponent.
		 */
		virtual void LoadContent();

		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime* gameTime);

		/**
		 * Derived Get_Type method from the CGameScreen.
		 */
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods

		/**
		 * Derived Activate method from the CGameScreen.
		 *
		 * @param	parentScreen	screen that this screen's activate is called from
		 */
		void Activate(CGameScreen*);

		/**
		 * Derived Deactivate method from the CGameScreen.
		 */
		void Deactivate();

		/**
		 * Shows the direction to the goal
		 */
		void ShowGuideArrow();
		#pragma endregion
};