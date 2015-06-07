/**
 * @description 
 *   Interface for the CScreenManager that allows the ability to manage screens. 
 *	 All the screen management, is handled by the class that implements this nterface.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

class CGameScreen;

class QUORUM_COMPONENTS_API IScreenManager : public IGameService
{
	public:
		#pragma region Overridden Properties

		/**
		 * Gets the type of the screen as a string. Base method that all screens will implement.
		 */
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods

		/**
		 * Adds the given screen to the collection of screens.
		 *
		 * @param	screen	screen to add to the collection of screens.
		 */
		virtual void AddScreen(CGameScreen* gameScreen) = 0;
		
		/**
		 * Removes the given screen from the collection of screens.
		 *
		 * @param	screen	screen to remove from the collection of screens.
		 */
		virtual void RemoveScreen(CGameScreen* gameScreen) = 0;

		/**
		 * Retrieves the screen with the name given. If no screen with the given name, return null.
		 *
		 * @param	screenName	name of the screen to retrieve
		 */
		virtual CGameScreen* GetScreen(char* screenName) = 0;
		#pragma region Methods
};