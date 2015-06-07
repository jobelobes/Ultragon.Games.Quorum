#include "3ngine.Graphics2D.h"

#pragma region Static Variables
ICameraService* CTiledPlane::g_pCameraService = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CTiledPlane::CTiledPlane(CGame* game, D3DXVECTOR2 mapsize)
:CDrawableGameComponent(game)
{
	this->m_eMode = TileScrollMode::WrapAround;
	this->m_pMesh = NULL;
	this->m_pEffect = NULL;	
	this->m_pTexture = NULL;

	ZeroMemory(&this->m_pMapPosition, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pMapSize, &mapsize, sizeof(D3DXVECTOR2));
	this->m_pTileSize.x = 1;
	this->m_pTileSize.y = 1;
	this->m_pRefPoint.x = this->m_pTileSize.x * 0.5f;
	this->m_pRefPoint.y = this->m_pTileSize.y * 0.5f;
	
	::D3DXMatrixRotationAxis(this->m_pRotation = new D3DXMATRIX(), &D3DXVECTOR3(1,0,0), 0);
	::D3DXMatrixTranslation(this->m_pTranslation = new D3DXMATRIX(), this->m_pRefPoint.x, 0, this->m_pRefPoint.y);
}

CTiledPlane::CTiledPlane(CGame* game, D3DXVECTOR3 normal, D3DXVECTOR2 mapsize)
:CDrawableGameComponent(game)
{
	this->m_eMode = TileScrollMode::WrapAround;
	this->m_pMesh = NULL;
	this->m_pEffect = NULL;	
	this->m_pTexture = NULL;

	ZeroMemory(&this->m_pMapPosition, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pMapSize, &mapsize, sizeof(D3DXVECTOR2));
	this->m_pTileSize.x = 1;
	this->m_pTileSize.y = 1;
	this->m_pRefPoint.x = this->m_pTileSize.x * 0.5f;
	this->m_pRefPoint.y = this->m_pTileSize.y * 0.5f;
	
	::D3DXMatrixRotationAxis(this->m_pRotation = new D3DXMATRIX(), &D3DXVECTOR3(1,0,0), 0);
	::D3DXMatrixTranslation(this->m_pTranslation = new D3DXMATRIX(), this->m_pRefPoint.x, 0, this->m_pRefPoint.y);
}

CTiledPlane::CTiledPlane(CGame* game, D3DXVECTOR2 mapsize, D3DXVECTOR2 tilesize)
:CDrawableGameComponent(game)
{
	this->m_eMode = TileScrollMode::WrapAround;
	this->m_pMesh = NULL;
	this->m_pEffect = NULL;	
	this->m_pTexture = NULL;

	ZeroMemory(&this->m_pMapPosition, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pTileSize, &tilesize, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pMapSize, &mapsize, sizeof(D3DXVECTOR2));
	this->m_pRefPoint.x = this->m_pTileSize.x * 0.5f;
	this->m_pRefPoint.y = this->m_pTileSize.y * 0.5f;
	
	::D3DXMatrixRotationAxis(this->m_pRotation = new D3DXMATRIX(), &D3DXVECTOR3(1,0,0), 0);
	::D3DXMatrixTranslation(this->m_pTranslation = new D3DXMATRIX(), this->m_pRefPoint.x, 0, this->m_pRefPoint.y);
}

CTiledPlane::CTiledPlane(CGame* game, D3DXVECTOR3 normal, D3DXVECTOR2 mapsize, D3DXVECTOR2 tilesize, TileScrollMode mode)
:CDrawableGameComponent(game)
{
	this->m_eMode = TileScrollMode::WrapAround;
	this->m_pMesh = NULL;
	this->m_pEffect = NULL;	
	this->m_pTexture = NULL;

	ZeroMemory(&this->m_pMapPosition, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pTileSize, &tilesize, sizeof(D3DXVECTOR2));
	memcpy(&this->m_pMapSize, &mapsize, sizeof(D3DXVECTOR2));
	this->m_pRefPoint.x = this->m_pTileSize.x * 0.5f;
	this->m_pRefPoint.y = this->m_pTileSize.y * 0.5f;
	
	::D3DXMatrixRotationAxis(this->m_pRotation = new D3DXMATRIX(), &D3DXVECTOR3(1,0,0), 0);
	::D3DXMatrixTranslation(this->m_pTranslation = new D3DXMATRIX(), this->m_pRefPoint.x, 0, this->m_pRefPoint.y);
}

CTiledPlane::~CTiledPlane()
{
	if(this->m_pMesh != NULL)
		delete this->m_pMesh;
}
#pragma endregion

#pragma region Properties
D3DXVECTOR2 CTiledPlane::Get_MapPosition()
{
	return this->m_pMapPosition;
}

void CTiledPlane::Set_MapPosition(D3DXVECTOR2 value)
{

}

CTexture2D* CTiledPlane::Get_Texture()
{
	return this->m_pTexture;
}

void CTiledPlane::Set_Texture(CTexture2D* value)
{
	this->m_pTexture = value;
}

#pragma endregion

#pragma region Methods
void CTiledPlane::BuildTile(VertexPositionColorTexture* vertices, UINT* indices, int* vIndex, int* iIndex, int x, int z)
{
	float xOffset = -this->m_pMapSize.x * this->m_pTileSize.x * 0.5f;
	float zOffset = -this->m_pMapSize.y * this->m_pTileSize.y * 0.5f;

	// create indices
	indices[(*iIndex)++] = *vIndex;
	indices[(*iIndex)++] = *vIndex + 1;
	indices[(*iIndex)++] = *vIndex + 2;

	indices[(*iIndex)++] = *vIndex + 2;
	indices[(*iIndex)++] = *vIndex + 1;
	indices[(*iIndex)++] = *vIndex + 3;

	// create vertices
	vertices[*vIndex].Position.x = xOffset + x * this->m_pTileSize.x;
	vertices[*vIndex].Position.y = 0;
	vertices[*vIndex].Position.z = zOffset + z * this->m_pTileSize.y;
	vertices[*vIndex].TextureCoord.x = 0;
	vertices[*vIndex].TextureCoord.y = 0;
	vertices[*vIndex].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	(*vIndex)++;

	vertices[*vIndex].Position.x = xOffset + (x + 1) * this->m_pTileSize.x;
	vertices[*vIndex].Position.y = 0;
	vertices[*vIndex].Position.z = zOffset + z * this->m_pTileSize.y;
	vertices[*vIndex].TextureCoord.x = 1;
	vertices[*vIndex].TextureCoord.y = 0;
	vertices[*vIndex].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	(*vIndex)++;

	vertices[*vIndex].Position.x = xOffset + x * this->m_pTileSize.x;
	vertices[*vIndex].Position.y = 0;
	vertices[*vIndex].Position.z = zOffset + (z + 1) * this->m_pTileSize.y;
	vertices[*vIndex].TextureCoord.x = 0;
	vertices[*vIndex].TextureCoord.y = 1;
	vertices[*vIndex].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	(*vIndex)++;

	vertices[*vIndex].Position.x = xOffset + (x + 1) * this->m_pTileSize.x;
	vertices[*vIndex].Position.y = 0;
	vertices[*vIndex].Position.z = zOffset + (z + 1) * this->m_pTileSize.y;
	vertices[*vIndex].TextureCoord.x = 1;
	vertices[*vIndex].TextureCoord.y = 1;
	vertices[*vIndex].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
	(*vIndex)++;
}

void CTiledPlane::BuildMesh()
{
	// delete old buffers
	if(this->m_pMesh != NULL)
		delete this->m_pMesh;

	int vertCount = this->m_pMapSize.x * this->m_pMapSize.y * 4;
	int indexCount = this->m_pMapSize.x * this->m_pMapSize.y * 6;

	// create temp buffers
	VertexPositionColorTexture* vertices = new VertexPositionColorTexture[vertCount];
	UINT* indices = new UINT[indexCount];

	// create directx buffers
	this->m_pMesh = new CMesh(this->Get_Game()->Get_GraphicsDevice());
		
	int vIndex = 0;
	int iIndex = 0;
	for(int z = 0; z < this->m_pMapSize.y; z++)
	{
		for(int x = 0; x < this->m_pMapSize.x; x++)
		{
			this->BuildTile(vertices, indices, &vIndex, &iIndex, x, z);
		}
	}

	// set buffer data
	this->m_pMesh->SetVertices(vertices, vertCount);
	this->m_pMesh->SetIndices(indices, indexCount);
	this->m_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	// delete temp buffers
	delete [] vertices;
	delete [] indices;
}

void CTiledPlane::UpdateTileTextures(D3DXVECTOR2 position)
{

}

void CTiledPlane::Scroll(D3DXVECTOR2 value)
{
	switch(this->m_eMode)
	{
		case TileScrollMode::WrapAround:
		{
			if(value.x != 0)
			{	
				this->m_pMapPosition.x = fmod(this->m_pMapPosition.x + value.x, this->m_pMapSize.x);
				if(this->m_pMapPosition.x < 0)
					this->m_pMapPosition.x = this->m_pTileSize.x - this->m_pMapPosition.x;
			}
			
			if(value.y != 0)
			{
				this->m_pMapPosition.y = fmod(this->m_pMapPosition.y + value.y, this->m_pMapSize.y);
				if(this->m_pMapPosition.y < 0)
					this->m_pMapPosition.y = this->m_pTileSize.y - this->m_pMapPosition.y;
			}

		}
		break;
		case TileScrollMode::Constrained:
		{
			this->m_pMapPosition.x += value.x;
			if(this->m_pMapPosition.x > (this->m_pMapSize.x * this->m_pTileSize.x))
				this->m_pMapPosition.x = (this->m_pMapSize.x * this->m_pTileSize.x);
			else if (this->m_pMapPosition.x < 0)
				this->m_pMapPosition.x = 0;

			this->m_pMapPosition.y += value.y;
			if(this->m_pMapPosition.y > (this->m_pMapSize.y * this->m_pTileSize.y))
				this->m_pMapPosition.y = (this->m_pMapSize.y * this->m_pTileSize.y);
			else if (this->m_pMapPosition.y < 0)
				this->m_pMapPosition.y = 0;
		}
		break;
	};

	if(value.x != 0)
	{
		this->m_pRefPoint.x = fmod(this->m_pRefPoint.x + value.x, this->m_pTileSize.x);
		if(this->m_pRefPoint.x < 0)
			this->m_pRefPoint.x = this->m_pTileSize.x - this->m_pRefPoint.x;
	}

	if(value.y != 0)
	{
		this->m_pRefPoint.y = fmod(this->m_pRefPoint.y + value.y, this->m_pTileSize.y);
		if(this->m_pRefPoint.y < 0)
			this->m_pRefPoint.y = this->m_pTileSize.y - this->m_pRefPoint.y;
	}
	
	::D3DXMatrixTranslation(this->m_pTranslation, this->m_pRefPoint.x, 0, this->m_pRefPoint.y);
	this->UpdateTileTextures(this->m_pMapPosition);
}

void CTiledPlane::ScrollTo(D3DXVECTOR2 value)
{
	this->m_pMapPosition.x = this->m_pMapSize.x / 2;
	this->m_pMapPosition.x = this->m_pMapSize.y / 2;
	this->Scroll(value);	
}
#pragma endregion

#pragma region Overriden Methods
void CTiledPlane::Initialize()
{
	CDrawableGameComponent::Initialize();

	if(CTiledPlane::g_pCameraService == NULL)
		CTiledPlane::g_pCameraService = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
}

void CTiledPlane::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	this->m_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultTextureEffect.fx");
	this->m_pMapPosition.x = this->m_pMapSize.x / 2;
	this->m_pMapPosition.x = this->m_pMapSize.y / 2;

	this->BuildMesh();
}

void CTiledPlane::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);
}

void CTiledPlane::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);
	D3DXMatrixMultiply(&w, &w, this->m_pTranslation);
	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	this->m_pEffect->Set_View(&CTiledPlane::g_pCameraService->Get_View());
	this->m_pEffect->Set_World(&w);
	this->m_pEffect->Set_Projection(&CTiledPlane::g_pCameraService->Get_Projection());
	if(this->m_pTexture != NULL)
		this->m_pEffect->SetVariable("InputTexture", this->m_pTexture);
	this->m_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = this->m_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			this->m_pMesh->Draw();
		}
	}
	this->m_pEffect->End();
}
#pragma endregion