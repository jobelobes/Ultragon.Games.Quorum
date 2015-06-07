/**
 * @description 
 *   Controls a basic unit in the game; this class is a base class that 
 *   other controllers use to extend how a unit is controlled
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CBasicUnitController : public CGameComponent, public IController
{
	private:
		#pragma region Variables
		IBasicUnit* m_pBoundUnit;
		ControllerMode m_eMode;
		#pragma endregion
		
	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Controller constructor
		 *
		 * @param	game	current game
		 */
		CBasicUnitController(CGame* game);

		/**
		 * Controller deconstructor
		 */
		~CBasicUnitController();
		#pragma endregion

		#pragma region IController Properties
		/**
		 * Returns whether the controller is bound to an object.
		 */
		virtual bool Get_IsBound();

		virtual ControllerMode Get_Mode();
		virtual void Set_Mode(ControllerMode);
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the bound object the controller is bound to.
		 */
		virtual IBasicUnit* Get_BoundObject();
		#pragma endregion

		#pragma region Methods
		/**
		 * Calls the bound objects Move with offset specified.
		 *
		 * @param	offset	amount of offset to move the object by
		 */
		virtual void Move(D3DXVECTOR2 offset);

		/**
		 * Calls the bound objects Rotate with offset specified.
		 *
		 * @param	offset	amount of offset to rotate the object by
		 */
		virtual void Rotate(float offset);

		/**
		 * Calls the bound objects Fire with the specified direction.
		 *
		 * @param	direction	direction to fire
		 */
		virtual void Fire(D3DXVECTOR2 dir);
		#pragma endregion

		#pragma region IController Methods
		/**
		 * Binds the controller to an object allowing the controller
		 *   to control the bound object.
		 *
		 * @param	object	unit that needs to bound to
		 */
		virtual void Bind(IControllableUnit* object);

		/**
		 * Unbinds the controller from the bound object.
		 */
		virtual void Unbind();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived initialize method from the CGameComponent.
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