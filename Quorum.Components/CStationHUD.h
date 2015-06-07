#include "Quorum.Components.h"

class CStation;

class QUORUM_COMPONENTS_API CStationHUD : public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static int g_iReferences;

		static CMesh* g_pMeshGear;
		static CMesh* g_pMeshAmmo;
		static CMesh* g_pMeshHealth;

		static CTexture2D* g_pTextureHUDGear;
		static CTexture2D* g_pTextureAmmo;
		static CTexture2D* g_pTextureAmmoMask;
		static CTexture2D* g_pTextureHealth;
		static CTexture2D* g_pTextureHealthMask;

		static CEffect* g_pEffect;
		static ICameraService* g_pCamera;
		#pragma endregion

		#pragma region Variables
		CStation* m_pStation;
		D3DXMATRIX* m_pGearWorld;
		D3DXMATRIX* m_pGearRotation;
		D3DXMATRIX* m_pGearTranslation;

		//transformation matrices of seperate modules
		D3DXMATRIX* m_pStationOffsetRotation;

		D3DXMATRIX* m_pHealthWorld;
		D3DXMATRIX* m_pHealthTranslation;

		D3DXMATRIX* m_pAmmoWorld;
		D3DXMATRIX* m_pAmmoTranslation;
		#pragma endregion

		#pragma region Methods
		void UpdateHealth(CGameTime*);
		void UpdateAmmo(CGameTime*);
		void UpdateGear(CGameTime*);

		void DrawHealth(CGraphicsDevice*, CGameTime*);
		void DrawAmmo(CGraphicsDevice*, CGameTime*);
		void DrawGear(CGraphicsDevice*, CGameTime*);
		#pragma endregion
	
	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Constructor that passes in a station to link to
		 *
		 * @param	game	current game object
		 * @param	station	station object that this hud will be linked to
		 */
		CStationHUD(CGame* game, CStation* station);

		/**
		 * StationHUD deconstructor
		 */
		~CStationHUD();
		#pragma endregion

		#pragma region Properties

		#pragma endregion

		#pragma region Static Methods
		/**
		 * Builds the static mesh for the Ship class
		 *
		 * @param	device	The graphics device that is registered to the game
		 * @param	scale	The scale used to displace the unit mesh
		 */
		static CMesh* BuildMesh(CGraphicsDevice* device, float scale);
		#pragma endregion

		#pragma region Overriden Methods
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