/**
 * @description 
 *   The ship is a composite of other objects, including turrets and stations.  Each
 *   child is in relation to the ship so that the movement and rotations are hiearchical.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CShip : public CGameUnit, public IBasicUnit, public IControllableUnit
{
	public:
		static IXACT3Wave* engineNoise;

	private:
		#pragma region Static Variables
		static const float InputMoveRate;
		static const float InputRotateRate;

		static int g_iReferences;

		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		static CTexture2D* g_pTexture;

		static ISceneManager* g_pSceneManager;
		static ICameraService* g_pCamera;
		static IWorldScroller* g_pWorldScroller;
		static ICollisionService* g_pCollision;
		#pragma endregion

		#pragma region Variables
		bool m_bBound;

		int m_iStationCount;
		CStation** m_pStations;

		float m_fRotation;
		D3DXVECTOR2 m_sPosition;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;
		#pragma endregion

		#pragma region Methods
		/**
		 * Builds the stations that make up the ship. These stations hold turrets, shield stations, and repair stations
		 *
		 * @param	count	the number of stations associated to the ship
		 */
		void BuildStations(int count);
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Builds the static mesh for the Ship class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice* device);
		#pragma endregion

	protected:
		#pragma region Properties
		/**
		 * Returns the world transform of the ship
		 */
		D3DXMATRIX* Get_Transform();

		/**
		 * Return the world coordinates of the ship
		 */
		D3DXVECTOR2 Get_WorldCoord();
		#pragma endregion

	public:
		#pragma region Properties
		bool Is_Breached();
		#pragma endregion

		#pragma region IBasicUnit Properties
		/**
		 * Derived from IBasicUnit
		 * Returns the move rate of the ship
		 */
		virtual float Get_MoveRate();
		
		/**
		 * Derived from IBasicUnit
		 * Returns the rotation rate of the ship
		 */
		virtual float Get_RotateRate();
		
		/**
		 * Derived from IBasicUnit
		 * returns the position of the ship
		 */
		virtual D3DXVECTOR2 Get_Position();
		
		/**
		 * Derived from IBasicUnit
		 * Sets the position of the ship
		 *
		 * @param	position	the position that the ship is being set to
		 */
		virtual void Set_Position(D3DXVECTOR2 position);

		virtual float Get_Rotation();

		virtual void Set_Rotation(float);
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

		#pragma region Overriden Properties
		virtual void Set_Enabled(bool);

		virtual void Set_Visible(bool);
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * Default constructor
		 *
		 * @param	game	current game
		 * @param	identifier for the ship
		 * @param	gameContent	game content object
		 */
		CShip(CGame* game, int identifier);

		/**
		 * Ship deconstructor
		 */
		~CShip();
		#pragma endregion

		#pragma region Methods

		/**
		 * Adds a turret to the ship
		 *
		 * @param	index	index where the turret will be added
		 * @param	turret	Turret object that is being added
		 */
		void AddTurret(int index, CTurret* turret);
		
		/**
		 * Returns the turret at a specific index
		 *
		 * @param	index	the index for the turret in question
		 */
		CTurret* GetTurretAt(int index);
		
		/**
		 * Removes a turret at a specific index
		 *
		 * @param	index	the index for the turret to be removed
		 */
		void RemoveTurret(int index);

		/**
		 * Returns the station at a specific index
		 * 
		 * @param	index	index of the station in question
		 */
		CStation* GetStationAt(int index);
		#pragma endregion
	
		#pragma region IBasicUnit Methods
		/**
		 * Derived from IBasicUnit
		 * Moves the ship based on an offset
		 *
		 * @param	offset	offset being added to the ship's current position
		 */
		virtual void Move(D3DXVECTOR2 offset);
		
		/**
		 * Derived from IBasicUnit
		 * Rotates the ship based on an angle offset
		 *
		 * @param	angle	angle to rotate the ship by
		 */
		virtual void Rotate(float angle);

		/**
		 * Derived from IBasicUnit
		 * Fire at the given target
		 *
		 * @param	target	target to fire at
		 */
		virtual void Fire(D3DXVECTOR2);
		#pragma endregion

		#pragma region Overridden Methods

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

		#pragma region Friend Classes
		friend class CMission;
		friend class CStation;
		friend class CPlayerCrewController;
		friend class CBaseCrewController;
		friend class CTurret;
		friend class CTarget;
		friend class CStationHUD;
		#pragma endregion
};