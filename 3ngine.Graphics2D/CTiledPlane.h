#include "3ngine.Graphics2D.h"

enum TileScrollMode
{
	WrapAround,
	Constrained
};

class GRAPHICS2D_API CTiledPlane: public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static ICameraService* g_pCameraService;
		#pragma endregion

		#pragma region Variables
		TileScrollMode m_eMode;

		D3DXVECTOR2 m_pTileSize;
		D3DXVECTOR2 m_pMapSize;
		D3DXVECTOR2 m_pMapPosition;
		D3DXVECTOR2 m_pRefPoint;

		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		CMesh* m_pMesh;
		CTexture2D* m_pTexture;
		CEffect* m_pEffect;
		ID3D10EffectVariable* m_pEffectTextureVar;
		#pragma endregion

		#pragma region Methods
		void BuildTile(VertexPositionColorTexture*, UINT*, int*, int*, int, int);
		#pragma endregion

	protected:
		#pragma region Methods
		virtual void BuildMesh();
		virtual void UpdateTileTextures(D3DXVECTOR2);
		#pragma endregion

	public:
		#pragma region Properties
		virtual D3DXVECTOR2 Get_MapPosition();
		void Set_MapPosition(D3DXVECTOR2 value);

		CTexture2D* Get_Texture();
		void Set_Texture(CTexture2D*);
		#pragma endregion

		#pragma region Constructors and Finalizers
		CTiledPlane(CGame*, D3DXVECTOR2);
		CTiledPlane(CGame*, D3DXVECTOR3, D3DXVECTOR2);
		CTiledPlane(CGame*, D3DXVECTOR2, D3DXVECTOR2);
		CTiledPlane(CGame*, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, TileScrollMode);
		~CTiledPlane();
		#pragma endregion

		#pragma region Methods
		virtual void Scroll(D3DXVECTOR2);
		virtual void ScrollTo(D3DXVECTOR2);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(CGameTime*);
		virtual void Draw(CGameTime*);
		#pragma endregion
};