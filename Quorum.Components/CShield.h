/**
 * @description 
 *   Basic shield; maintains the global mesh and a list of textures(for each type of bullet)
 *   and draw its position in relation to its given position and initial velocity.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */
#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CShield : public CGameUnit
{

	private:
		#pragma region Static Variables
		static int g_iReferences;
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
				 
		static CTexture2D* g_pTexture;
		static CTexture2D* g_pNoiseTexture;
		static CTexture2D* g_pShieldMask;

		static ICameraService* g_pCamera;
		static ICollisionService* g_pCollision;
		static ISceneManager* g_pSceneManager;
		#pragma endregion

		#pragma region Variables
		float m_fShieldRotation;
		float m_fOffX;
		float m_fOffZ;
		
		bool m_bBound;
		bool m_bIncrementUp;

		int m_iColor;

		D3DXVECTOR2 m_pPosition;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		float m_fNoiseX;
		float m_fNoiseY;

		CGameUnit* m_pOwner;
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Builds the static mesh for the bullet class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice* device);
		#pragma endregion

	public:
		#pragma region Static Variables
		static const float InputRotateRate;
		#pragma endregion

		#pragma region Constructors and Deconstructors
		/**
		 * Default constructor
		 *
		 * @param	game	current game object
		 */
		CShield(CGame*, int);

		/**
		 * Default shield deconstructor
		 */
		~CShield();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the world transform of the shield object
		 */
		D3DXMATRIX* Get_Transform();

		/**
		 * Return the rotate rate of the shield object
		 */
		float Get_RotateRate();

		/**
		 * Return the position of the shield
		 */
		D3DXVECTOR2 Get_Position();

		/**
		 * Return the owner of the shield so we can test collision detection
		 */
		CGameUnit* Get_Owner();

		/**
		 * Set the owner of the shield so we can test collistion detection
		 *
		 * @param	unit	unit to set the owner of the shield to
		 */
		void Set_Owner(CGameUnit* unit);

		/**
		 * Return whether the shield is bound to a station
		 */
		bool Get_Bound();

		/**
		 * Set whether or not the shield is bound by a crew member right now
		 *
		 * @param	value	flag that will be set for the shield's bound variable
		 */
		void Set_Bound(bool value);
		#pragma endregion

		#pragma region ICollidable Methods
		/**
		 * Derived from ICollidable object.
		 * Checks to see if a collision has occured with this bullet and any other object that inherits ICollidable
		 *
		 * @param	unit	the unit that the bullet is checking against
		 */
		virtual bool CollisionOccured(ICollidableUnit* unit);
		#pragma endregion

		#pragma region Methods
		/**
		 * Rotates the shield based on an angle offset that is passed in
		 * 
		 * @param	angle	angle offset passed in
		 */
		void Rotate(float angle);

		/**
		 * Rotates the shield based on an angle offset that is passed in
		 * 
		 * @param	angle	angle offset passed in
		 */
		void Set_Color(int color);
		#pragma endregion

		#pragma region Overidden Methods

		/**
		 * Derived from CGameUnit
		 * Initializes the Shield object
		 */
		virtual void Initialize();
				
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