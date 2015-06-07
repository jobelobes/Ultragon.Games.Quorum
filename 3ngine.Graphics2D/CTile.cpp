#include "3ngine.Graphics2D.h"

#pragma region Static Variables
CMesh* CTile::g_pMesh = NULL;
CEffect* CTile::g_pEffect = NULL;
#pragma endregion

#pragma region Properties
D3DXVECTOR2* CTile::Get_Size()
{
	return this->m_pSize;
}

void CTile::Set_Size(D3DXVECTOR2* value)
{
	this->m_pSize = value;
}

LONG CTile::Get_Width()
{
	return this->m_pSize->x;
}

void CTile::Set_Width(LONG value)
{
	this->m_pSize->x = value;
}

LONG CTile::Get_Height()
{
	return this->m_pSize->y;
}

void CTile::Set_Height(LONG value)
{
	this->m_pSize->y = value;
}

D3DXVECTOR3* CTile::Get_Position()
{
	D3DXVECTOR3* pt;
	return pt;
}

void CTile::Set_Position(D3DXVECTOR3* value)
{
	D3DXMatrixTranslation(this->m_pTranslation, value->x, value->y, value->z);
}
#pragma endregion

#pragma region Constructors and Finalizers
CTile::CTile(CGame* game) 
 : CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pRotation = new D3DXMATRIX();
	this->m_pTranslation = new D3DXMATRIX();
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(1,0,0), 0);
	D3DXMatrixTranslation(this->m_pTranslation, 0,0,0);
}

CTile::CTile(CGame* game, D3DXVECTOR3* position)
: CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pRotation = new D3DXMATRIX();
	this->m_pTranslation = new D3DXMATRIX();
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(1,0,0), 0);
	D3DXMatrixTranslation(this->m_pTranslation, position->x, position->y, position->z);
}

CTile::CTile(CGame* game, D3DXVECTOR3* position, D3DXVECTOR2* size)
: CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pRotation = new D3DXMATRIX();
	this->m_pTranslation = new D3DXMATRIX();
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(1,0,0), 0);
	D3DXMatrixTranslation(this->m_pTranslation, position->x, position->y, position->z);
}

CTile::CTile(CGame* game, D3DXVECTOR3* position, D3DXVECTOR3* normal)
: CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pRotation = new D3DXMATRIX();
	this->m_pTranslation = new D3DXMATRIX();
	float angle = D3DXVec3Dot(&D3DXVECTOR3(0,0,1), normal);
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(1,0,0), -D3DX_PI/ 2);
	D3DXMatrixTranslation(this->m_pTranslation, position->x, position->y, position->z);
}

CTile::CTile(CGame* game, D3DXVECTOR3* position, D3DXVECTOR3* normal, D3DXVECTOR2* size)
: CDrawableGameComponent(game)
{
	this->m_pCamera = NULL;
	this->m_pRotation = new D3DXMATRIX();
	this->m_pTranslation = new D3DXMATRIX();
	float angle = D3DXVec3Dot(normal, &D3DXVECTOR3(0,0,1));
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(1,0,0), angle);
	D3DXMatrixTranslation(this->m_pTranslation, position->x, position->y, position->z);
}

CTile::~CTile(){}
#pragma endregion

#pragma region Overriden Methods
void CTile::Initialize()
{
	CDrawableGameComponent::Initialize();

	this->m_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(this->m_pCamera == NULL)
		exit(-1);
}

void CTile::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	if(CTile::g_pMesh == NULL)
		CTile::g_pMesh = CTile::BuildMesh(this->Get_Game()->Get_GraphicsDevice());

	if(CTile::g_pEffect == NULL)
		CTile::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultEffect.fx");
}

void CTile::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);
	D3DXMatrixMultiply(&w, &w, this->m_pRotation);
	D3DXMatrixMultiply(&w, &w, this->m_pTranslation);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor.VertexElements, VertexPositionColor::VertexElementCount);

	CTile::g_pEffect->Set_View(&this->m_pCamera->Get_View());
	CTile::g_pEffect->Set_World(&w);
	CTile::g_pEffect->Set_Projection(&this->m_pCamera->Get_Projection());
	CTile::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CTile::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CTile::g_pMesh->Draw();
		}
	}
	CTile::g_pEffect->End();
}
#pragma endregion

#pragma region Static Methods
CMesh* CTile::BuildMesh(CGraphicsDevice* device)
{
	// create vertices
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.5f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -0.5f;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;
	vertices[0].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	vertices[1].Position.x = 0.5f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -0.5f;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;
	vertices[1].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	vertices[2].Position.x = -0.5f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = 0.5f;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;
	vertices[2].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	vertices[3].Position.x = 0.5f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 0.5f;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;
	vertices[3].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);

	CMesh* mesh = new CMesh(device);
	mesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	mesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	mesh->SetAttributes((UINT*)attributes, 2);
	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
	return mesh;
}