/**
 * @description 
 *   Controls the crew member and provides the basic controls for moving
 *   around the ship and controlling the turrets
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CBaseCrewController : public CGameComponent, public IController
{
	private:
		#pragma region Variables
		CBasicUnitController* m_pShipController;
		CTurretController* m_pTurretController;
		CCrew* m_pBoundUnit;

		ControllerMode m_eMode;
		#pragma endregion

	protected:
		#pragma region Nested Classes
		struct CREW_PACKET
		{
			short int Index;
			StationType Type;
			D3DXVECTOR2 Position;
			float Rotation;
		};
		#pragma endregion

		#pragma region Static Variables
		static INetworkService* g_pNetwork;
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the crew object that is bound to this controller
		 */
		virtual CCrew* Get_BoundUnit();
		
		/**
		 * Returns this controller's ship controller
		 */
		virtual CBasicUnitController* Get_ShipController();

		/**
		 * Returns this controller's turret controller
		 */
		virtual CTurretController* Get_TurretController();
		#pragma endregion

		#pragma region Methods
		void NetworkUpdate(CGameTime*, int);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		/**
		 * Default CrewController constructor
		 *
		 * @param	game	current game object
		 */
		CBaseCrewController(CGame* game);
		
		/**
		 * CrewController Deconstructor
		 */
		~CBaseCrewController();
		#pragma endregion 

		#pragma region IController Properties
		/**
		 * Derived from IController
		 * Returns whether or not this controller is bound to an object
		 */
		virtual bool Get_IsBound();

		virtual ControllerMode Get_Mode();
		virtual void Set_Mode(ControllerMode);
		#pragma endregion

		#pragma region IController Methods
		
		/**
		 * Derived from IController
		 * Binds this controller to a crew member object
		 *
		 * @param	unit	the unit that this controller will be bound to
		 */
		void Bind(IControllableUnit* unit);

		/**
		 * Derived from IController
		 * Unbinds this controller from its current unit
		 */
		virtual void Unbind();
		#pragma endregion

		#pragma region Methods
		/**
		 * Calls for the turret controller's fire method
		 */
		void Fire();

		/**
		 * Calls for the bound unit's move method. Can be either a ship or crew member itself
		 * Takes an offset vector and passes it through
		 * 
		 * @param	offset	vector to offset the position by
		 */
		void Move(D3DXVECTOR2);

		/**
		 * Calls for the bound unit's rotate method. Can be either a ship or a turret
		 * The call to which sub controller (turret or ship) will depend on the type of station the crew member is currently bound it
		 *
		 * @param	angle	angle to offset the rotation by
		 * @param	type	type of station the crew member is currently bound ti
		 */
		void Rotate(float angle, StationType type);

		/**
		 * Calls for the bound unit's set_color method. Can be a shield
		 * The call to which sub controller (turret or ship) will depend on the type of station the crew member is currently bound it
		 *
		 * @param	color	int representint turret/bullet type
		 */
		void Set_Color(int color);

		/**
		 * Switches the stations based on input. Takes in an index to call to the ship to get which station to switch to
		 *
		 * @param	index	index of the station to switch to
		 */
		virtual void SwitchStation(int index);
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		void Initialize();
		#pragma endregion
};