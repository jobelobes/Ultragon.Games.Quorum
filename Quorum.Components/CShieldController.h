/**
 * @description 
 *   Controls the ship's shield; this class is used inconjunction with the CBaseCrewController.
 *   Once a controller is bound to the turret, the turret can fire bullets and rotate.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CShieldController : public CGameComponent, public IController
{
	private:
		#pragma region Variables
		CTurret* m_pBoundUnit;

		IInputService* m_pInputService;
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default Shield Controller Constructor
		 *
		 * @param	game	current game object
		 */
		CShieldController(CGame*);

		/**
		 * Shield controller deconstructor
		 */
		~CShieldController();
		#pragma endregion

		#pragma region IController Properties
		/**
		 * Returns whetherornot this controller is bound to a unit
		 */
		bool Get_IsBound();
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
		 * Unbinds the shield controller from its currently bound unit
		 */
		virtual void Unbind();
		#pragma endregion

		#pragma region Methods
		/**
		 * Calls the rotate for this controller's bound unit and passes it to the unit itself
		 *
		 * @param	angle	angle offset to rotate the shield by
		 */
		void Rotate(float angle);
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