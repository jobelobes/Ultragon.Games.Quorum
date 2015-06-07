/**
 * @description 
 *   Controls the enemy unit; at the moment this controller just determines
 *   a destination and moves towards it, then rechooses another destination.
 *   This will be replaced with a functional AI controller to manage the enemies
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CBaseEnemyController : public CBasicUnitController
{
	protected:
		#pragma region Nested Classes
		struct ENEMY_PACKET
		{
			D3DXVECTOR2 Position;
			float Rotation;
			int Health;
		};
		#pragma endregion

		#pragma region Static Variables
		static INetworkService* g_pNetwork;
		#pragma endregion

		#pragma region Methods
		void NetworkUpdate(CGameTime*, int);
		#pragma endregion

		#pragma region Constructors and Deconstructors
		/*
		 * Default Constructor
		 *
		 * @param	game	Current game object
		 */
		CBaseEnemyController(CGame* game);
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		~CBaseEnemyController();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived from IControllableUnit
		 * Binds the controller to a unit
		 *
		 * @param	unit	unit the controller is being bound to	
		 */
		virtual void Bind(IControllableUnit* unit);

		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		virtual void Initialize();
		#pragma endregion
};