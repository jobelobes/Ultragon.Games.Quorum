/**
 * @description 
 *   An extension of the CBaseCrewController, this class is the users primary means of 
 *   controlling the turrets, ship and crew member.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CPlayerCrewController : public CBaseCrewController
{
	private:
		#pragma region Variables
		int m_iStationIndex;
		CTarget* m_pTarget;

		IInputService* m_pInputService;
		#pragma endregion

		#pragma region Methods
		/**
		 * Handles the windows input for the user
		 *
		 * @param	inputService	WindowsInputService that retrieves the keyboard and mouse states
		 * @param	gameTime		game time of game currently running
		 */
		void HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime);
		
		/**
		 * Handles the XBox controller input. Controller scheme is based off XBox360 controller
		 *
		 * @param	inputService	XBoxInputService that retrieve the controller state
		 * @param	gameTime		game time of the game currently running
		 */
		void HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime);
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors

		/**
		 * Default User Controller Constructor
		 *
		 * @param	game	current game object
		 */
		CPlayerCrewController(CGame* game);

		/**
		 * User Controller Deconstructor
		 */
		~CPlayerCrewController();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the target reticule associated with the player
		 */
		CTarget* Get_Target();
		#pragma endregion

		#pragma region Methods
		/**
		 * Creates a target object for the player to aim with
		 */
		void CreateTarget();

		/**
		 * Moves the player's target reticule based on an offset from the control
		 *
		 * @param	offset	offset vector passed to the target
		 */
		void MoveTarget(D3DXVECTOR2 offset);

		virtual void SwitchStation(int);
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived from IController
		 * Unbinds this controller from its current unit
		 */
		void Unbind();

		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		void Initialize();
		
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		void Update(CGameTime*);
		#pragma endregion
};