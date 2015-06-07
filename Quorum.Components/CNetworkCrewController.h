/**
 * @description 
 *   An extension of the CBaseCrewController, this class is the network's primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CNetworkCrewController : public CBaseCrewController
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
		CNetworkCrewController(CGame* game);
		/**
		 * User Controller Deconstructor
		 */
		~CNetworkCrewController();
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