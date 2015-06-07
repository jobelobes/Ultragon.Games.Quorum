#include "3ngine.Core.h"

class CORE_API CBoundingPolygon
{
	private:
		#pragma region Static Variables
		static CEffect* g_pEffect;
		static CTexture2D* g_pTexture;
		#pragma endregion

		#pragma region Variables
		std::vector<D3DXVECTOR2> m_pSegments;
		D3DXMATRIX m_sTransform;
		CMesh* g_pMesh;
		#pragma endregion

		#pragma region Methods
		ContainmentType CheckLineIntersection(D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2);
		#pragma endregion

		#pragma region Static Methods
		static CMesh* BuildMesh(CGraphicsDevice*, std::vector<D3DXVECTOR2>*);
		#pragma endregion

	public:
		#pragma region Variables
		D3DXVECTOR3 Min;
		D3DXVECTOR3 Max;
		#pragma endregion

		#pragma region Constructors and Finalizers
		CBoundingPolygon();
		~CBoundingPolygon();
		#pragma endregion

		#pragma region Methods
		ContainmentType Contains(CBoundingPolygon*);
		ContainmentType Contains(CBoundingBox*);
		ContainmentType Contains(D3DXVECTOR3);
		bool Intersects(CBoundingPolygon);
		void Draw(CGraphicsDevice*, ICameraService*);
		#pragma endregion

		#pragma region Opertator Overriden Methods
		bool operator==(const CBoundingPolygon& value);
		bool operator!=(const CBoundingPolygon& value);
		#pragma endregion
};
