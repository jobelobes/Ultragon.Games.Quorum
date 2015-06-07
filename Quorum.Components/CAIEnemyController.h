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

enum AIMode{ Mode0, Mode1, Mode2 };

class QUORUM_COMPONENTS_API CAIEnemyController : public CBaseEnemyController
{
	private:
		#pragma region Variables
		AIMode m_eMode;
		D3DXVECTOR2 m_sGoalPosition;
		float m_circleAngle;
		bool m_bIsCircling;
		bool m_bTowardShip;
		D3DXVECTOR2 m_sCurrVelocity;
		bool m_bIsInitial;
		int state;
		#pragma endregion

		#pragma region Methods
		void CirclingController(CGameTime*, int);
		void RandomController(CGameTime*, int);
		void KamikazeController(CGameTime*, int);
		void CalcRotationAndRotate(D3DXVECTOR2*);
		#pragma endregion

	public:
		#pragma region Overriden Properties
		virtual ControllerMode Get_Mode();
		#pragma endregion

		#pragma region Constructors and Deconstructors
		CAIEnemyController(CGame* game);
		/*
		 * Default Constructor
		 *
		 * @param	game	Current game object
		 * @param	mode	mode set for the enemy controller
		 */
		CAIEnemyController(CGame* game, AIMode mode);

		~CAIEnemyController();
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
		virtual void Update(CGameTime*);
		#pragma endregion
};