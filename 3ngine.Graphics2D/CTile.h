#include "3ngine.Graphics2D.h"

class CTiledPlane;

class GRAPHICS2D_API CTile : public CDrawableGameComponent
{
	private:
		#pragma region Static Variables
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		#pragma endregion

		#pragma region Variables
		CTexture2D* g_pTexture;

		D3DXVECTOR2* m_pSize;
		D3DXMATRIX* m_pTranslation;
		D3DXMATRIX* m_pRotation;

		ICameraService* m_pCamera;
		#pragma endregion

		#pragma region Methods
		static CMesh* BuildMesh(CGraphicsDevice*);
		#pragma endregion
	public:
		#pragma region Constructors and Finalizers
		CTile(CGame*);
		CTile(CGame*, D3DXVECTOR3*);
		CTile(CGame*, D3DXVECTOR3*, D3DXVECTOR2*);
		CTile(CGame*, D3DXVECTOR3*, D3DXVECTOR3*);
		CTile(CGame*, D3DXVECTOR3*, D3DXVECTOR3*, D3DXVECTOR2*);
		~CTile();
		#pragma endregion

		#pragma region Properties
		D3DXVECTOR2* Get_Size();
		void Set_Size(D3DXVECTOR2*);

		LONG Get_Width();
		void Set_Width(LONG);

		LONG Get_Height();
		void Set_Height(LONG);

		D3DXVECTOR3* Get_Position();
		void Set_Position(D3DXVECTOR3*);

		CTexture2D* Get_Texture();
		void Set_Texture(CTexture2D*);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Draw(CGameTime*);
		#pragma endregion

		friend class CTiledPlane;
};