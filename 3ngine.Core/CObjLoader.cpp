#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CObjLoader::CObjLoader()
{
	this->m_pVertices = NULL;
	this->m_pIndices = NULL;
	this->m_pAttributes = NULL;
	this->m_pMaterials = NULL;
	this->m_cVertexCount = 0;
	this->m_cIndexCount = 0;
}

CObjLoader::~CObjLoader()
{
	if(this->m_pVertices != NULL)
		delete [] this->m_pVertices;
	if(this->m_pIndices != NULL)
		delete [] this->m_pIndices;
	if(this->m_pAttributes != NULL)
		delete [] this->m_pAttributes;
	if(this->m_pMaterials != NULL)
		delete [] this->m_pMaterials;
}
#pragma endregion

#pragma region Properties
VertexPositionNormalTexture* CObjLoader::Get_Vertices()
{
	return this->m_pVertices;
}

UINT* CObjLoader::Get_Indices()
{
	return this->m_pIndices;
}

UINT* CObjLoader::Get_Attributes()
{
	return this->m_pAttributes;
}

CMaterial* CObjLoader::Get_Materials()
{
	return this->m_pMaterials;
}

UINT CObjLoader::Get_VertexCount()
{
	return this->m_cVertexCount;
}

UINT CObjLoader::Get_IndexCount()
{
	return this->m_cIndexCount;
}

UINT CObjLoader::Get_AttributeCount()
{
	return this->m_cAttributeCount;
}

UINT CObjLoader::Get_MaterialCount()
{
	return this->m_cMaterialCount;
}	
#pragma endregion

#pragma region Methods
HRESULT CObjLoader::LoadObjGeometry(std::vector<VertexPositionNormalTexture>* vertices, std::vector<UINT>* indices, std::vector<UINT>* attributes, char* path)
{
	std::vector<D3DXVECTOR3> positions;
    std::vector<D3DXVECTOR2> texCoords;
    std::vector<D3DXVECTOR3> normals;
	std::vector<CacheEntry> cache;

	WCHAR cmd[256] = {0};
	std::wifstream stream(path);
    if( !stream )
        return E_FAIL;

	UINT subsets = 0;
	while(true)
    {
        stream >> cmd;
        if( !stream )
            break;

        if( 0 == wcscmp( cmd, L"v" ) ) // Vertex Position
        {
            float x, y, z;
            stream >> x >> y >> z;
			positions.push_back( D3DXVECTOR3( x, y, z ) );
        }
        else if( 0 == wcscmp( cmd, L"vt" ) ) // Vertex TexCoord
        {
            float u, v;
            stream >> u >> v;
            texCoords.push_back( D3DXVECTOR2( u, v ) );
        }
        else if( 0 == wcscmp( cmd, L"vn" ) ) // Vertex Normal
        {
            float x, y, z;
            stream >> x >> y >> z;
            normals.push_back( D3DXVECTOR3( x, y, z ) );
        }
        else if( 0 == wcscmp( cmd, L"f" ) ) // Face
        {
            int position, texCoord, normal;
			VertexPositionNormalTexture vertex;

            for( int face = 0; face < 3; face++ )
            {
				::ZeroMemory(&vertex, sizeof(VertexPositionNormalTexture) );

                stream >> position;
                vertex.Position = positions[position - 1];

                if( '/' == stream.peek() )
                {
                    stream.ignore();

                    if( '/' != stream.peek() ) // Optional texture coordinate
                    {
                        stream >> texCoord;
                        vertex.TextureCoord = texCoords[texCoord - 1];
                    }
                    if( '/' == stream.peek() ) // Optional vertex normal
                    {
                        stream.ignore();
						stream >> normal;
                        vertex.Normal = normals[normal - 1];
                    }
                }

				vertices->push_back(vertex);
                indices->push_back((UINT)vertices->size() );
            }
            attributes->push_back(subsets);
        }
        /*else if( 0 == wcscmp( cmd, L"mtllib" ) )
        {
            stream >> strMaterialFilename;
        }
        else if( 0 == wcscmp( cmd, L"usemtl" ) )
        {
            WCHAR strName[MAX_PATH] = {0};
            stream >> strName;

            bool bFound = false;
            for( int iMaterial = 0; iMaterial < m_Materials.GetSize(); iMaterial++ )
            {
                Material* pCurMaterial = m_Materials.GetAt( iMaterial );
                if( 0 == wcscmp( pCurMaterial->strName, strName ) )
                {
                    bFound = true;
                    dwCurSubset = iMaterial;
                    break;
                }
            }

            if( !bFound )
            {
                pMaterial = new Material();
                if( pMaterial == NULL )
                    return E_OUTOFMEMORY;

                dwCurSubset = m_Materials.GetSize();

                InitMaterial( pMaterial );
                StringCchCopy( pMaterial->strName, MAX_PATH - 1, strName );

                m_Materials.Add( pMaterial );
            }
        }*/
        stream.ignore( 1000, '\n' );
    }
    stream.close();

	return S_OK;
}

HRESULT CObjLoader::LoadMaterialsFromMTL(std::vector<CMaterial>* materials, char* path)
{
	return S_OK;
}
#pragma endregion

#pragma region Static Methods
void CObjLoader::LoadMesh(char* path)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> attributes;
	std::vector<CMaterial> materials;

	HRESULT hr = CObjLoader::LoadObjGeometry(&vertices, &indices, &attributes, path);
	hr = CObjLoader::LoadMaterialsFromMTL(&materials, path);

	// set the vertices data
	this->m_cVertexCount = vertices.size();
	this->m_pVertices = new VertexPositionNormalTexture[this->m_cVertexCount];
	copy(vertices.begin(), vertices.end(), this->m_pVertices);

	// Set the index data
	this->m_cIndexCount = indices.size();
	this->m_pIndices = new UINT[this->m_cIndexCount];
	copy(indices.begin(), indices.end(), this->m_pIndices);

	// Set the attribute data
	this->m_cAttributeCount = attributes.size();
	this->m_pAttributes = new UINT[this->m_cAttributeCount];
	copy(attributes.begin(), attributes.end(), this->m_pAttributes);

	// Set the material data
	this->m_cMaterialCount = materials.size();
	this->m_pMaterials = new CMaterial[this->m_cMaterialCount];
	copy(materials.begin(), materials.end(), this->m_pMaterials);
}
#pragma endregion