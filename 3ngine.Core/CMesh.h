#include "3ngine.Core.h"

class CORE_API CMesh
{
	private:
		CGraphicsDevice* m_pDevice;
		
		void* m_pVertices;
		UINT* m_pIndices;
		UINT* m_pAttributes;
		UINT m_cVertexCount;
		UINT m_cIndexCount;
		UINT m_cAttributeCount;

	public:
		ID3D10Device* m_pGraphicsDevice;
		ID3DX10Mesh* m_pMesh;

		CMesh(CGraphicsDevice*);
		~CMesh();

		#pragma region Static Methods
		static CMesh* LoadFile(CGraphicsDevice*, char*);
		#pragma endregion

		#pragma region Methods
		void SetVertices(void*, UINT);
		void SetIndices(UINT*, UINT count);
		void SetAttributes(UINT*, UINT count);
		void Create(D3D10_INPUT_ELEMENT_DESC*, int);
		void Draw();
		#pragma endregion
};