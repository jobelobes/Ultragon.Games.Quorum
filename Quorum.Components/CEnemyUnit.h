/**
 * @description 
 *   This is the enemy unit of the game.  The class can be bound to via an AI
 *   controller for dynamic movement and behaviors.  The quintisential enemy of the game.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

struct EnemyAttributes
{
	int Type;
	int FireRadius;
	int Health;
	int Damage;
	float Cooldown;
};

class QUORUM_COMPONENTS_API CEnemyUnit : public CGameUnit, public IBasicUnit, public IControllableUnit
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static CMesh* g_pMesh;
		static CEffect*	g_pEffect;
		static CTexture2D* g_pTexture;

		static ICameraService* g_pCamera;
		static IWorldScroller* g_pWorldScroller;
		static ICollisionService* g_pCollision;
		static CQuorumSceneManager* g_pSceneManager;
		#pragma endregion

		#pragma	region Variables
		bool m_bBound;
		bool m_bIsHit;

		bool m_bIsOkToFire;
		float m_fFireTimer;
		float m_fHitCountdown;

		D3DXVECTOR2 m_sPosition;
		float m_fRotation;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pTranslation;
		
		EnemyAttributes m_sAttributes;

		CShip* m_pTarget;
		#pragma endregion

		#pragma region static Methods
		/**
		 * Builds the static mesh for the Enemy unit class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice*);
		#pragma endregion

	public:
		#pragma region Static Variables
		static const float InputMoveRate;
		static const float InputRotateRate;
		#pragma endregion

		#pragma region Properties

		/**
		 * Returns the ship that the enemy is targetting
		 */

		CShip* Get_Target();
		
		/**
		 * Sets the ship target
		 *
		 * @param	ship	ship target
		 */
		void Set_Target(CShip* ship);

		/**
		 * Return the health attribute from the EnemyAttributes struct
		 */
		int Get_Health();

		/**
		 * Sets the health of the unit
		 *
		 * @param	value	value to de-increment the health by
		 */
		void Set_Health(int value);

		/**
		 * Returns the damage that the ship is capable of. Translated to the bullet's it fires
		 */
		int Get_Damage();

		/**
		 * Returns the fire radius of the ship
		 */
		int Get_FireRadius();

		/**
		 * Returns the speed of the ship
		 */
		int Get_Speed();

		/**
		 * Returns the type of enemy
		 */
		int Get_Type();

		/**
		 * Returns whether or not the enemy can fire
		 */
		bool Get_IsOkToFire();
		
		/**
		 * Sets whether or not the enemy can fire
		 */
		void Set_IsOkToFire(bool);

		/**
		 * Gets the current rotation angle
		 */
		float Get_RotationAngle();

		#pragma endregion

		#pragma region IBasicUnit Properties
		/**
		 * Derived from IBasicUnit
		 * Returns the move rate of the enemy
		 */
		virtual float Get_MoveRate();
		
		/**
		 * Derived from IBasicUnit
		 * Returns the rotation rate of the enemy
		 */
		virtual float Get_RotateRate();


		/**
		 * Derived from IBasicUnit
		 * returns the position of the enemy
		 */
		virtual D3DXVECTOR2 Get_Position();
		
		/**
		 * Derived from IBasicUnit
		 * Sets the position of the enemy
		 *
		 * @param	position	the position that the enemy is being set to
		 */
		virtual void Set_Position(D3DXVECTOR2);

		/**
		 * Derived from IBasicUnit
		 * Gets the rotation of the enemy
		 */
		virtual float Get_Rotation();

		/**
		 * Derived from IBasicUnit
		 * Sets the rotation of the enemy
		 *
		 * @param	rotation	the rotation that the enemy is being set to
		 */
		virtual void Set_Rotation(float value);
		#pragma endregion

		#pragma region IControllableUnit Properties

		/**
		 * Derived from IControllableUnit
		 * Returns a boolean checking whether the ship is bound to a controller or not
		 */
		virtual bool Get_IsBound();

		/**
		 * Derived from IControllableUnit
		 * Sets the boolean flag for if the ship object is bound to a controller or not
		 * 
		 * @param	isBound	flag to set the ship to
		 */
		virtual void Set_IsBound(bool isBound);
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * Default constructor
		 *
		 * @param	game	current game
		 */
		CEnemyUnit(CGame* game, int identifier);

		/**
		 * Enemy unit deconstructor
		 */
		~CEnemyUnit();
		#pragma endregion

		#pragma region IBasicUnit Methods
		/**
		 * Derived from IBasicUnit
		 * Moves the ship based on an offset
		 *
		 * @param	offset	offset being added to the ship's current position
		 */
		void Move(D3DXVECTOR2);

		/**
		 * Derived from IBasicUnit
		 * Rotates the ship based on an angle offset
		 *
		 * @param	angle	angle to rotate the ship by
		 */
		void Rotate(float);

		/**
		 * Derived from IBasicUnit
		 * Fire at the given target
		 *
		 * @param	target	target to fire at
		 */
		void Fire(D3DXVECTOR2);
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived from ICollidableUnit
		 * Checks to see if a collision has occured with another object
		 *
		 * @param	unit	the collidable unit the be checked against
		 */
		virtual bool CollisionOccured(ICollidableUnit* unit);

		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived LoadContent method from the CGameComponent.
		 */
		virtual void LoadContent();
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime*);

		/**
		 * Derived Draw method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion

		friend class CBaseEnemyController;
		friend class MainScreen;
};