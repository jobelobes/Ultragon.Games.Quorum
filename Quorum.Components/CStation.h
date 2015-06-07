/**
 * @description 
 *   Since the ship is composed of stations, this class implements the basic needs
 *   of a single station.  On the ship object, this station is rotated around the center
 *   to create the hull.  This object is also the parent object of the turret.  The station
 *   type defined whether the station can maintain a turret and what type of turret.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

enum StationType : unsigned short int
{
	Pilot = 0,
	Turret = 1,
	Shield = 2,
	Engineering = 3
};

struct StationAttributes
{
	StationType Type;
	int ID;
	int Health;
	int Armor;
	int TurretType;
};


class CShip;

class QUORUM_COMPONENTS_API CStation: public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static int g_iStationCount;

		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		static CTexture2D* g_pSegmentTexture;
		static CTexture2D* g_pActiveTexture;
		static CTexture2D** g_pStationTexture;

		static ICameraService* g_pCamera;
		static ICollisionService* g_pCollision;
		static ISceneManager* g_pSceneManager;
		#pragma endregion

		#pragma region Variables
		bool m_bIsBound;
		CShip* m_pShip;
		CTurret* m_pTurret;
		CStationHUD* m_pHUD;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pTranslation;

		StationAttributes m_sAttributes;
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Loads up an array of textures to be used for turrets
		 *
		 * @param	device	graphics device registered to the game
		 * @param	size	number of textures to be loaded
		 */
		static CTexture2D** LoadTextures(CGraphicsDevice* device, int size);

		/**
		 * Builds the static mesh for the Ship class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice*);
		#pragma endregion

		#pragma region Methods
		/**
		 * Creates a new turret objcet and links it to this station object
		 */
		void BuildTurret();
		#pragma endregion

	public:
		#pragma region Properties
		/**
		 * Returns this station's world transform
		 */
		D3DXMATRIX* Get_Transform();

		/**
		 * Returns whether or not a turret is bound to a controller
		 */
		bool Get_IsBound();

		/**
		 * Sets whether or not the turret is bound to a controller
		 *
		 * @param	value	boolean passed in to set the isBound flag
		 */
		void Set_IsBound(bool value);

		/**
		 * Returns the movement rate for the object that is linked to this station
		 */
		int Get_MoveRate();

		/**
		 * Returns the rotation rate for the object that is linked to this station
		 */
		float Get_RotateRate();

		/**
		 * Returns the turret object that is linked to this station (if there is one)
		 */
		CTurret* Get_Turret();

		/**
		 * Returns the type of station this station object is
		 */
		StationType Get_StationType();

		/**
		 * Returns the ship object that this station is linked to
		 */
		CShip* Get_Ship();

		void Set_HUDEnabled(bool);
		#pragma endregion

		#pragma region Overriden Properties
		void CStation::Set_Enabled(bool value)
		{
			CGameComponent::Set_Enabled(value);

			if(this->m_pTurret != NULL)
				this->m_pTurret->Set_Enabled(value);
		}

		void CStation::Set_Visible(bool value)
		{
			CDrawableGameComponent::Set_Visible(value);

			if(this->m_pTurret != NULL)
				this->m_pTurret->Set_Visible(value);
		}
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * Constructor that passes in a ship to link to, a station type, a translation matrix, and a rotation matrix
		 *
		 * @param	game		current game object
		 * @param	gameContent	custom loaded game content
		 * @param	ship		ship object that this station will be linked to
		 * @param	translation initial translation matrix for the station
		 * @param	rotation	initial rotation matrix for the station
		 * @param	attributes	custom loaded station attributes
		 */
		CStation(CGame* game, CShip* ship, D3DXMATRIX translation, D3DXMATRIX rotation, StationAttributes attributes);
		
		/**
		 * Station deconstructor
		 */
		~CStation();
		#pragma endregion

		#pragma region CGameUnit Methods
		/**
		 * Derived from ICollidableUnit
		 * Checks to see if a collision has occured with another object
		 *
		 * @param	unit	the collidable unit the be checked against
		 */
		//virtual bool CollisionOccured(ICollidableUnit* unit);
		#pragma endregion

		#pragma region Overriden Methods
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

		friend class CShip;
		friend class CTurret;
		friend class CStationHUD;
};