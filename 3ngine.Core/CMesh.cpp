#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CMesh::CMesh(CGraphicsDevice* device)
{
	this->m_cVertexCount = 0;
	this->m_pVertices = NULL;
	this->m_cIndexCount = 0;
	this->m_pIndices = NULL;
	this->m_cAttributeCount = 0;
	this->m_pAttributes = NULL;

	this->m_pDevice = device;
	this->m_pGraphicsDevice = this->m_pDevice->m_pGraphicsDevice;
}

CMesh::~CMesh()
{

}
#pragma endregion

#pragma region Methods
void CMesh::SetVertices(void* vertices, UINT count)
{
	this->m_cVertexCount = count;
	this->m_pVertices = vertices;
}

void CMesh::SetIndices(UINT* indices, UINT count)
{
	this->m_cIndexCount = count;
	this->m_pIndices = indices;
}

void CMesh::SetAttributes(UINT* attributes, UINT count)
{
	this->m_cAttributeCount = count;
	this->m_pAttributes = attributes;
}


void CMesh::Create(D3D10_INPUT_ELEMENT_DESC* elements, int elementsCount)
{
	HRESULT hr = D3DX10CreateMesh(this->m_pDevice->m_pGraphicsDevice,
						elements,
						elementsCount,
                        elements[0].SemanticName,
                        this->m_cVertexCount,
                        1,
                        D3DX10_MESH_32_BIT,
                        &this->m_pMesh );


    hr = this->m_pMesh->SetVertexData( 0, this->m_pVertices);
    hr = this->m_pMesh->SetIndexData(this->m_pIndices, this->m_cIndexCount);
	if(this->m_pAttributes != NULL)
	{
		hr = this->m_pMesh->SetAttributeData(this->m_pAttributes);
		hr = this->m_pMesh->GenerateAdjacencyAndPointReps(0.01f);
	}
    
	hr = this->m_pMesh->Optimize(D3DX10_MESHOPT_COMPACT | D3DX10_MESHOPT_ATTR_SORT | D3DX10_MESHOPT_VERTEX_CACHE, NULL, NULL);
    hr = this->m_pMesh->CommitToDevice();
}

void CMesh::Draw()
{
	HRESULT hr;
	hr = this->m_pMesh->CommitToDevice();
	for(UINT i = 0; i < 1; i++)
		hr = this->m_pMesh->DrawSubset(i);
}
#pragma endregion

#pragma region Static Methods
CMesh* CMesh::LoadFile(CGraphicsDevice* device, char* path)
{
	CMesh* model = new CMesh(device);

	CObjLoader* loader = new CObjLoader();
	loader->LoadMesh(path);

	HRESULT hr = D3DX10CreateMesh(device->m_pGraphicsDevice,
						VertexPositionColorTexture::VertexElements,
						VertexPositionColorTexture::VertexElementCount,
                        VertexPositionColorTexture::VertexElements[0].SemanticName,
                        loader->Get_VertexCount(),
                        loader->Get_IndexCount(),
                        D3DX10_MESH_32_BIT,
                        &model->m_pMesh );


    hr = model->m_pMesh->SetVertexData( 0, loader->Get_Vertices());
    hr = model->m_pMesh->SetIndexData(loader->Get_Indices(), loader->Get_IndexCount());
    hr = model->m_pMesh->SetAttributeData(loader->Get_Attributes());
    hr = model->m_pMesh->GenerateAdjacencyAndPointReps(0.01f);
	hr = model->m_pMesh->Optimize(D3DX10_MESHOPT_COMPACT | D3DX10_MESHOPT_ATTR_SORT | D3DX10_MESHOPT_VERTEX_CACHE, NULL, NULL);
    hr = model->m_pMesh->CommitToDevice();

	delete loader;
	return model;
}
#pragma endregion