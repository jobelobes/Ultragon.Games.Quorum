/**
 * @description 
 *   Basic bullet; maintains the global mesh and a list of textures(for each type of bullet)
 *   and draw its position in relation to its given position and initial velocity.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class CQuorumSceneManager;

enum BulletType : unsigned short int { Hellfire = 0, Swivel = 1, Machety = 2, BigBertha = 3};
enum BulletOwner : unsigned short int { Owner_Ship, Owner_Enemy };

class QUORUM_COMPONENTS_API CBullet : public CGameUnit
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
				
		static int g_iTextureCount;
		static CTexture2D** g_pTextures;

		static ICameraService* g_pCamera;
		static ICollisionService* g_pCollision;
		static IWorldScroller* g_pWorldScroller;
		static CQuorumSceneManager* g_pSceneManager;
		#pragma endregion

		#pragma region Variables
		BulletOwner m_eOwner;
		int m_iImpactDamage;
		float m_fAge;
		float m_fLifespan;

		D3DXVECTOR2 m_sVelocity;
		D3DXVECTOR2 m_sPosition;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		CAnimation* m_pAnimation;

		BulletType m_iType;
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Loads an array of textures for the bullet. 
		 * Depending on the type of turret that the bullet is associated with will determine which texture gets used later on
		 *
		 * @param	device	The graphics device that is registered to the game
		 * @param	size	the number of textures related to the bullet object. 
		 */
		static CTexture2D** LoadTextures(CGraphicsDevice* device, int size);
		/**
		 * Builds the static mesh for the bullet class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice* device);
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default constructor
		 *
		 * @param	game	current game
		 * @param	identifier	Quorum Identifier
		 */
		CBullet(CGame*, int identifier);
		/**
		 * Bullet deconstructor
		 */
		~CBullet();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the bullet's owner game unit object
		 */
		BulletOwner Get_Owner();

		/**
		 * Sets the Bullet's owner
		 *
		 * @param	value	CGameUnit object that is being set to this bullet's m_pOwner variable
		 */
		void Set_Owner(BulletOwner value);

		/**
		 * Returns the bullet's position
		 */
		D3DXVECTOR2 Get_Position();

		/**
		 * Sets the position of the bullet object
		 *
		 * @param	position	position that the bullet is being set to
		 */
		void Set_Position(D3DXVECTOR2 position);

		/**
		 * Returns the velocity of the bullet
		 */
		D3DXVECTOR2 Get_Velocity();
		
		/**
		 * Sets the velocity of the bullet
		 *
		 * @param	velocity	velocity that the bullet is being set to
		 */
		void Set_Velocity(D3DXVECTOR2 velocity);

		/**
		 * Returns the impact damage of the bullet
		 */
		int Get_Damage();

		/**
		 * Sets the damage of the bullet
		 *
		 * @param	value	value of the damage
		 */
		void Set_Damage(int value);

		/**
		 * Return the type of the bullet
		 */
		BulletType Get_BulletType();

		/**
		 * Set the type of bullet
		 *
		 * @param	value	type of bullet
		 */
		void Set_BulletType(BulletType value);

		/**
		 * Returns the animation for the bullet
		 */
		CAnimation* Get_Animation();

		/**
		 * Return the lifespan of the bullet
		 */
		float Get_Lifespan();

		/**
		 * Set the lifespan of the bullet
		 *
		 * @param	value	`value passed in
		 */
		void Set_Lifespan(float value);
		#pragma endregion

		#pragma region Methods
		/**
		 * Initializes the animation for the bullet
		 */
		void InitializeAnimation();
		#pragma endregion

		#pragma region Overidden Methods

		/**
		 * Derived from ICollidable object.
		 * Checks to see if a collision has occured with this bullet and any other object that inherits ICollidable
		 *
		 * @param	unit	the unit that the bullet is checking against
		 */
		virtual bool CollisionOccured(ICollidableUnit* unit);
		
		/**
		 * Derived from CGameUnit
		 * Loads content associated to the bullet such as textures, effects, etc
		 */
		virtual void LoadContent();

		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime* gameTime);
		
		/**
		 * Derived from CGameUnit
		 * Draws the bullet on the screen and applies effects if necessary
		 *
		 * @param	gameTime	game time of the game
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion
};