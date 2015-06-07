/**
 * @description 
 *   An extension of the CBaseEnemyController, this class is the network's primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CNetworkEnemyController : public CBaseEnemyController
{
	public:
		#pragma region Overriden Properties
		virtual ControllerMode Get_Mode();
		#pragma endregion

		#pragma region Constructors and Deconstructors
		/**
		 * Default User Controller Constructor
		 *
		 * @param	game	current game object
		 */
		CNetworkEnemyController(CGame* game);
		/**
		 * User Controller Deconstructor
		 */
		~CNetworkEnemyController();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		void Update(CGameTime*);
		#pragma endregion
};