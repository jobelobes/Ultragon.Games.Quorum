/**
 * @description 
 *   Class for the targetting reticule for the turrets. The reticule will be allowed to roam around in 
 *	  freespace while the turret follows it around
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CTarget : public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static const float InputMoveRate;
		static const float InputRotateRate;
		
		static int g_iReferences;
		static int g_iTextureCount;
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		static CTexture2D* g_pTexture;

		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables
		bool m_bIsTurretBound;

		D3DXVECTOR2* m_pPosition;

		D3DXMATRIX* m_pWorld;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		D3DXMATRIX* m_pShipTransform;
		#pragma endregion

		#pragma region Static Methods
		/**
		 * Builds the static mesh for the Target class
		 *
		 * @param	device	The graphics device that is registered to the game
		 */
		static void BuildMesh(CGraphicsDevice* device);
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default Target Constructor
		 *
		 * @param	game	current game object
		 * @param	turret	turret object that this target is linked to
		 * @param	rotation	default rotation that the target will be set to
		 * @param	shipTransform	ship object's world tranformation matrix
		 */
		CTarget(CGame*,D3DXMATRIX translation, D3DXMATRIX rotation, D3DXMATRIX* shipTransform);
		
		/**
		 * Target Deconstructor
		 */
		~CTarget();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the target's world transform
		 */
		D3DXMATRIX* Get_Transform();

		/**
		 * Returns whether or not the target is turret bound. Associated to whether or not we will draw the target
		 */
		bool Get_IsTurretBound();

		/**
		 * Sets the flag for whether the target is bound to a turret station
		 *
		 * @param	value	passed in boolean flag to set the turret bound flag to
		 */
		void Set_IsTurretBound(bool value);

		float Get_MoveRate();
		#pragma endregion

		#pragma region Methods
		void Move(D3DXVECTOR2);
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived Initialize method from the CDrawableGameComponent.
		 */
		virtual void Initialize();

		/**
		 * Derived LoadContent method from the CDrawableGameComponent.
		 */
		virtual void LoadContent();
		/**
		 * Derived Update method from the CDrawableGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Update(CGameTime*);

		/**
		 * Derived Draw method from the CDrawableGameComponent.
		 *
		 * @param	gameTime	the games elapsed/total time
		 */
		virtual void Draw(CGameTime*);
		#pragma endregion
};