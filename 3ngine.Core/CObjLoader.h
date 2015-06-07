#include "3ngine.Core.h"

class VertexPositionNormalTexture;
class CMaterial;

class CORE_API CObjLoader
{
	private:
		#pragma region Nested Classes
		struct CacheEntry
		{
			int index;
			CacheEntry* pNext;
		};
		#pragma endregion

		#pragma region Variables
		VertexPositionNormalTexture* m_pVertices;
		UINT* m_pIndices;
		UINT* m_pAttributes;
		CMaterial* m_pMaterials;
		UINT m_cVertexCount;
		UINT m_cIndexCount;
		UINT m_cAttributeCount;
		UINT m_cMaterialCount;
		#pragma endregion

		#pragma region Static Methods
		HRESULT LoadObjGeometry(std::vector<VertexPositionNormalTexture>*, std::vector<UINT>*, std::vector<UINT>*, char*);
		HRESULT LoadMaterialsFromMTL(std::vector<CMaterial>*, char*);
		#pragma endregion
	public:
		CObjLoader();
		~CObjLoader();

		#pragma region Properties
		VertexPositionNormalTexture* Get_Vertices();
		UINT* Get_Indices();
		UINT* Get_Attributes();
		CMaterial* Get_Materials();
		UINT Get_VertexCount();
		UINT Get_IndexCount();
		UINT Get_AttributeCount();
		UINT Get_MaterialCount();
		#pragma endregion

		#pragma region Methods
		void LoadMesh(char*);
		#pragma endregion
};