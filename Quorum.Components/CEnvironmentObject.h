/**
* @description 
*   Objects that are not player controlled ships or enemy ships. Represents objects such as asteroids and space junk
*
* @version 1.0.0.0
* @author	Eric Moreau
*
*/


#include "Quorum.Components.h"

// We should really change this so the data comes in from XML!
enum EnvironmentObjectType
{
	Asteroid = 0,
	AsteroidNormal = 1,
	Planet = 2,
	SpaceStation = 3,
	Nebula = 4,
	Size = 5 // should always be the last and appropriate value of the enum
};

class QUORUM_COMPONENTS_API CEnvironmentObject : public CGameUnit
{	
	private:
		#pragma region StaticVariables

		static int g_iReferences;
		static CEffect* g_pEffect;
				
		static int g_iTextureCount;
		static int g_iNebulaCount;

		static CMesh** g_pMeshes;
		static CTexture2D** g_pTextures;
		static CTexture2D** g_pNebulaTextures;
		static CTexture2D** g_pPlanetTextures;

		static ICameraService* g_pCamera;
		static ICollisionService* g_pCollision;
		static IWorldScroller* g_pWorldScroller;
		static ISceneManager* g_pSceneManager;	

		#pragma endregion

		#pragma region Variables
		EnvironmentObjectType m_iObjectType;

		D3DXVECTOR2 m_sPosition;
		D3DXVECTOR2 m_pScale;

		float m_fRotation;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pScaling;

		CAnimation* m_pAnimation;

		int m_iFrameNumber;
		D3DXVECTOR2 m_pTextureSize;
		D3DXVECTOR2 m_pImageSize;

		D3DXVECTOR4 m_pPlanetColor;
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		/**
		 * Default constructor
		 *
		 * @param	game	current game object
		 * @param	type	object type
		 */
		CEnvironmentObject(CGame* game,EnvironmentObjectType type);
		
		/**
		 * Deconstructor object
		 */
		~CEnvironmentObject();
		#pragma endregion

		#pragma region Properties
		/**
		 * Return the object type
		 */
		EnvironmentObjectType Get_ObjectType();
		/**
		 * Set the object type of the current environment object
		 *
		 * @param	type	passed in EnvironmentObjectType type
		 */
		void Set_ObjectType(EnvironmentObjectType type);

		/**
		 * Returns the position of the environment object
		 */
		D3DXVECTOR2 Get_Position();

		/**
		 * Set the position of the environment object
		 *
		 * @param	value	position passed in
		 */
		void Set_Position(D3DXVECTOR2);

		/**
		 * Return the rotation of the object
		 */
		float Get_Rotation();

		/**
		 * Set the rotation of the object
		 *
		 * @param	value	float value for rotation offset
		 */
		void Set_Rotation(float value);

		/**
		 * Return the scale of the animation image
		 */
		D3DXVECTOR2 Get_Scale();

		/**
		 * Set a custom scale for the animation image
		 *
		 * @param	value	vector2 scale
		 */
		void Set_Scale(D3DXVECTOR2 value);

		/**
		 * Return the animation for the object
		 */
		CAnimation* Get_Animation();
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Loads up an array of textures to be used for objects
		 *
		 * @param	device	graphics device registered to the game
		 * @param	size	number of textures to be loaded
		 */
		static CTexture2D** LoadTextures(CGraphicsDevice* device, int size);

		/**
		 * Loads the textures specific to the nebulas
		 *
		 * @param	device	graphics device for the game
		 * @param	size	number of textures to load in
		 */
		static CTexture2D** LoadNebulas(CGraphicsDevice* device, int size);
		
		/**
		 * Loads the textures specific to the nebulas
		 *
		 * @param	device	graphics device for the game
		 * @param	size	number of textures to load in
		 */
		static CTexture2D** LoadPlanets(CGraphicsDevice* device, int size);
		/**
		 * Builds and returns a mesh for drawring objects
		 *
		 * @param	device	graphics device registered to the game
		 * @param	scaleX  the size/ratio in the X direction
		 * @param	scaleZ  the size/ratio in the Z direction
		 */
		static CMesh* BuildMesh(CGraphicsDevice*, float scaleX, float scaleZ);

		/**
		 * Loads up an array of meshes to be used for objects
		 *
		 * @param	device	graphics device registered to the game
		 * @param	size	number of meshes to be loaded
		 */
		static CMesh** LoadMeshes(CGraphicsDevice*, int size);
		#pragma endregion

		#pragma region Static Properties
		/**
		 * Returns the mesh for an environment object
		 *
		 * @param	type	specified type of object for the inquired mesh
		 */
		static CMesh* Get_Mesh(EnvironmentObjectType type);

		/**
		 * Returns the texture for a given environment object type
		 *
		 * @param	type	specified type of object for the inquired mesh
		 */
		static CTexture2D* Get_Texture(EnvironmentObjectType type);
		#pragma endregion

		#pragma region Methods
		/**
		 * Initialize the animation for the asteroid if there is one
		 */
		void InitializeAnimation();

		/**
		 * Update the world based on the ship's position
		 */
		void UpdateWorld();
		#pragma endregion

		#pragma region Overridden Methods
		/**
		 * Checks the collision with a given ICollidableUnit
		 *
		 * @param	unit	unit to test the collision with
		 */
		virtual bool CollisionOccured(ICollidableUnit*);
		
		virtual void Initialize();
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void LoadContent();
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime*);
		/**
		 * Derived Update method from the CGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion
};