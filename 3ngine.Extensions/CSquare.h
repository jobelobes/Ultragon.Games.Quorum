#include "3ngine.Extensions.h"

class EXTENSIONS_API CSquare
{
	private:
		#pragma region Static Variables
		static CMesh* g_pMesh;
		static CEffect* g_pEffect;
		static CTexture2D* g_pTexture;
		#pragma endregion

		#pragma region Methods
		static CMesh* BuildMesh(CGraphicsDevice*);
		#pragma endregion

	public:
		#pragma region Methods
		static void Draw(D3DXCOLOR, D3DXVECTOR3, D3DXVECTOR3, CGraphicsDevice*, ICameraService*);
		#pragma endregion
};

