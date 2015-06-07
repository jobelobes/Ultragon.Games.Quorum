/**
 * @description 
 *   Controls the ship; this class is used inconjunction with the CBaseCrewController.
 *   Once a controller is bound to the ship, the movement of the ship around the
 *   world become active and the user is able to manipulate the ships position.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CShipController : public CBasicUnitController
{
	private:
		#pragma region Variables
		CShip* m_pBoundUnit;
		#pragma endregion

		#pragma region Methods
		void NetworkUpdate(CGameTime*, int);
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default Ship Controller Constructor
		 *
		 * @param	game	current game object
		 */
		CShipController(CGame*);
		
		/**
		 * Ship Controller Deconstructor
		 */
		~CShipController();
		#pragma endregion

		#pragma region CBasicUnitController Methods
		
		/**
		 * Derived from CBasicUnitController
		 * Binds this ship controller object to a ship object
		 *
		 * @param	unit	ship object to bind to
		 */
		void Bind(IControllableUnit* unit);

		/**
		 * Derived from CBasicUnitController
		 * Unbinds this controller from its currently bound ship object
		 */
		void Unbind();
		#pragma endregion

		#pragma region Methods
		
		/**
		 * Moves this controller's currently bound unit based on a vector offset
		 *
		 * @param	offset	offset vector to adjust the bound unit's position
		 */
		void Move(D3DXVECTOR2);
		
		/**
		 * Rotate's the controller's currently bound unit based on an angle offset
		 *
		 * @param	angle	angle offset to rotate the bound unit by
		 */
		void Rotate(float);
		#pragma endregion
};