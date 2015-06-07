/**
 * @description 
 *   Controls the ship's turrets; this class is used inconjunction with the CBaseCrewController.
 *   Once a controller is bound to the turret, the turret can fire bullets and rotate.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CTurretController : public CGameComponent, public IController
{
	private:
		#pragma region Variables
		CTurret* m_pBoundUnit;

		IInputService* m_pInputService;
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default Turret Controller Constructor
		 *
		 * @param	game	current game object
		 */
		CTurretController(CGame*);
		
		/**
		 * Turret Controller Deconstructor
		 */
		~CTurretController();
		#pragma endregion

		#pragma region IController Properties
		/**
		 * Returns whether or not this controller is bound to a unit
		 */
		virtual bool Get_IsBound();

		virtual ControllerMode Get_Mode();
		virtual void Set_Mode(ControllerMode);
		#pragma endregion

		#pragma region IController Methods
		/**
		 * Derived from IController
		 * Binds this turret controller to a turret object
		 *
		 * @param	unit	turret object to bind this controller to
		 */
		virtual void Bind(IControllableUnit* unit);

		/**
		 * Derived from IController
		 * Unbinds this turret controller from its currently bound unit
		 */
		virtual void Unbind();
		#pragma endregion

		#pragma region Methods
		/**
		 * Calls the fire method of this controller's bound unit.
		 */
		void Fire();

		/**
		 * Calls the rotate method of this controller's bound unit and passes an angle offset
		 *
		 * @param	angle	angle offset to adjust the rotation by
		 */
		void Rotate(float angle);

		/**
		 * Calls the rotate shield method of this controller's bound unit and passes it an angle offset
		 *
		 * @param	angle	angle offset to adjust the shield by
		 */
		void RotateShield(float angle);

		void Set_Color(int color);
		#pragma endregion

		#pragma region Overriden Methods
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