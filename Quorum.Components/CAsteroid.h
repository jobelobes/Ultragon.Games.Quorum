/**
* @description 
*   Objects that are not player controlled ships or enemy ships. Represents objects such as asteroids and space junk
*
* @version 1.0.0.0
* @author	Eric Moreau
*
*/


#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CAsteroid : public CEnvironmentObject, public ICollidableUnit
{	
	private:
		#pragma region StaticVariables

		static int g_iReferences;
		static CEffect* g_pEffect;
				
		static int g_iTextureCount;
		static CMesh* g_pMesh;
		static CTexture2D* g_pTexture;
		static CTexture2D* g_pNormalTexture;

		static ICameraService* g_pCamera;
		static ICollisionService* g_pCollision;
		static IWorldScroller* g_pWorldScroller;
		static ISceneManager* g_pSceneManager;	

		#pragma endregion

		#pragma region Variables
		D3DXVECTOR3* m_pVelocity;
		D3DXVECTOR3* m_pRelPosition;

		CAnimation* m_pAnimatedAsteroid;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		EnvironmentObjectType m_iObjectType;
		CBoundingBox* m_pBoundingBox;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CAsteroid(CGame*);
		~CAsteroid();
		#pragma endregion

		#pragma region Properties
		/**
		 * Return the position of the asteroid
		 */
		D3DXVECTOR3* Get_Position();

		/**
		 * Set the position of the asteroid
		 *
		 * @param	position	custom position passed in
		 */
		void Set_Position(D3DXVECTOR2* position);
		#pragma endregion

		#pragma region Methods
		/**
		 * Initializes the Turret CAnimation with the correct values for our spritesheet
		 */
		void InitializeAsteroidAnimation();
		#pragma endregion

		#pragma region ICollidable Properties
		/**
		 * Derived from ICollidableUnit
		 * Returns the bounding box of the asteroid
		 */
		CBoundingBox* Get_BoundingBox();
		#pragma endregion

		#pragma region ICollidableUnit Methods
		/**
		* Dervived from ICollidableUnit
		*
		* @param	unit	collidable unit to be tested against
		*/
		virtual bool CollisionOccured(ICollidableUnit* unit);
		#pragma endregion

		#pragma region Overridden Methods

		/**
		 * Derived Initialize method from the CGameComponent.
		 */
		virtual void Initialize();

		/*
		 * Derived LoadContent method from CDrawableGameComponent
		 */
		virtual void LoadContent();

		/**
		 * Derived Update method from CDrawableGameComponent
		 *
		 * @param	gameTime	game object's gametime
		 */
		virtual void Update(CGameTime* gameTime);
		
		/**
		 * Derived Draw method from CDrawableGameComponent
		 *
		 * @param	gameTime	game object's gametime
		 */
		virtual void Draw(CGameTime* gameTime);
		#pragma endregion
};