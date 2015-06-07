/**
 * @description 
 *   Represents the turrets that display.  This object can fire bullets and rotate
 *   around its axis point.  Also manages what texture to display on the turret.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

struct TurretAttributes
{
	char* Name;
	int Type;
	float Cooldown;
	float Speed;
	int Damage;
	float Lifespan;
};

class CStation;
class CQuorumSceneManager;

class QUORUM_COMPONENTS_API CTurret : public CDrawableGameComponent, public IControllableUnit
{
	private:
		#pragma region Static Variables
		static int g_iReferences;

		static CMesh* g_pMesh;
		static CMesh* g_pShieldMesh;

		static CEffect* g_pEffect;
		static CEffect* g_pShieldEffect;

		static int g_iTextureCount;
		static CTexture2D** g_pTextures;
		static CTexture2D* g_pShieldTexture;

		static ICameraService* g_pCamera;
		static CQuorumSceneManager* g_pSceneManager;
		static IWorldScroller* g_pWorldScroller;
		#pragma endregion

		#pragma region Variables
		bool m_bBound;
		bool m_bIsOkToFire;
		float m_fFireTimer;
		
		CAnimation* m_pAnimatedTurret;

		CStation* m_pShipSegment;
		CTarget* m_pTarget;
		CShield* m_pShield;
		
		float m_pBarrelRotation;
		D3DXMATRIX* m_pFireTarget;
		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pRotation;
		D3DXMATRIX* m_pTranslation;

		TurretAttributes m_sAttributes;
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
		 * @param	scaleX	scale along the X axis
		 * @param	scaleZ	scale along the Z axis
		 */
		static CMesh* BuildMesh(CGraphicsDevice*, float scaleX, float scaleZ);
		#pragma endregion

	public:
		#pragma region Static Variables
		static const float InputRotateRate;
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Return the lifespan of the bullet based on a passed in type index
		 *
		 * @param	type	type of turret asked for
		 */
		static float Get_StaticLifespan(int type);
		#pragma endregion

		#pragma region Constructors and Finalizers
		/**
		 * Constructor that passes in a custom position and rotation, as well as a Station segment to be linked to
		 *
		 * @param	game	current game object
		 * @param	shipsegment	station segment that the turret is linked to
		 * @param	translation	custom position to set the turret to
		 * @param	rotation	custom rotation to set the turret as
		 * @param	attributes	custom loaded attributes for this turret
		 */
		CTurret(CGame* game, CStation* shipsegment, D3DXMATRIX translation, D3DXMATRIX rotation, TurretAttributes attributes);

		/**
		 * Turret deconstructor
		 */
		~CTurret();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the type of turret this object is. Directly linked to StationType struct
		 */
		int Get_Type();
		
		/**
		 * Sets the type of turret this object is
		 *
		 * @param	type	type represented as an int to set the turret as
		 */
		void Set_Type(int type);

		/**
		 * Returns the cooldown of the turret
		 */
		int Get_Cooldown();

		/**
		 * Sets the cooldown of the turret
		 *
		 * @param	value	cooldown passed in as an int
		 */
		void Set_Cooldown(int value);

		/**
		 * Returns the world tranform of the turret
		 */
		D3DXMATRIX* Get_Transform();

		/**
		 * Set the transform of the turret
		 *
		 * @param	transform	matrix that the tranform will be set to
		 */
		void Set_Transform(D3DXMATRIX transform);
		/**
		 * Returns the rotation rate of the turret
		 */

		float Get_RotateRate();

		/**
		 * Returns the matrix of the fire target of the turret
		 */
		D3DXMATRIX* Get_FireTarget();

		/**
		 * Returns the station that this turret is attached to
		 */
		CStation* Get_Station();

		/**
		 * Returns the shield object related to this turret (if there is one)
		 */
		CShield* Get_Shield();

		float Get_BarrelRotation();

		void Set_BarrelRotation(float);
		#pragma endregion

		#pragma region DrawableGameComponent Properties
		virtual void Set_Enabled(bool);

		virtual void Set_Visible(bool);
		#pragma endregion

		#pragma region IControllableUnit Properties
		/**
		 * Derived from IControllableUnit
		 * Returns whether or not a turret is bound to a controller
		 */
		virtual bool Get_IsBound();

		/**
		 * Derived from IControllableUnit
		 * Sets whether or not the turret is bound to a controller
		 *
		 * @param	value	boolean passed in to set the isBound flag
		 */
		virtual void Set_IsBound(bool value);
		#pragma endregion

		#pragma region Methods
		/**
		 * Initializes the Turret CAnimation with the correct values for our spritesheet
		 */
		void InitializeTurretAnimation();


		/**
		 * Triggers a fire event that creates a new bullet object and launches it
		 */
		void Fire();

		/**
		 * Rotates the turret based on an angle offset passed in
		 *
		 * @param	angle	angle offset passed in to rotate the turret's world transform by
		 */
		void Rotate(float angle);

		/**
		 * Rotates the turret based on an angle offset passed in
		 *
		 * @param	angle	angle passed in to rotate the turret's world transform by
		 */
		void RotateAbs(float angle);

		/**
		 * Rotates the shield based on an angle offset
		 *
		 * @param	angle	angle offset to add to the current rotation of the shield
		 */
		void RotateShield(float angle);
		
		void Set_Color(int color);

		/**
		 * Binds the turret to a target so we can track its position
		 *
		 * @param	target	target object that the turret will be bound to
		 */
		void BindTarget(CTarget* target);

		/**
		 * Unbinds the target from the turret'
		 */
		void UnbindTarget();
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
};