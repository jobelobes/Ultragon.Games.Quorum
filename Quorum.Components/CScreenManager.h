/**
 * @description 
 *   Manages all screens that are created. Allows screens to be added, removed and 
 *	 retrieved so that all screens are maintained in one central location.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CScreenManager : public CGameComponent, public IScreenManager
{
	private:
		#pragma region Variables
		std::vector<CGameScreen*> m_pGameScreens;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		
		/**
		 * ScreenManager Constructor.
		 *
		 * @game	game	current game
		 */
		CScreenManager(CGame* game);
		
		/**
		 * ScreenManager Destructor.
		 */
		~CScreenManager();
		#pragma endregion

		#pragma region IScreen Methods
		
		/**
		 * Derived AddScreen method from the IScreenManager.
		 *
		 * @param	screen	screen to add to the collection of screens.
		 */
		virtual void AddScreen(CGameScreen* screen);
		
		/**
		 * Derived RemoveScreen method from the IScreenManager.
		 *
		 * @param	screen	screen to remove from the collection of screens.
		 */
		virtual void RemoveScreen(CGameScreen* screen);
		
		/**
		 * Derived GetScreen method from the IScreenManager.
		 *
		 * @param	screenName	name of the screen to retrieve
		 */
		virtual CGameScreen* GetScreen(char* screenName);
		#pragma region Methods

		#pragma region Overridden Methods
		
		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime* gameTime);
		#pragma endregion
};