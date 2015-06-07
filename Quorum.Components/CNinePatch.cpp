/**
 * @description 
 *   Draws the basic window structure using a 9-patch texture
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CNinePatch::g_iReferences = 0;
CMesh* CNinePatch::g_pMesh = NULL;
CEffect* CNinePatch::g_pEffect = NULL;

ICameraService* CNinePatch::g_pCamera = NULL;
#pragma endregion

#pragma region Properties
D3DXVECTOR2 CNinePatch::Get_Position()
{
	return *this->m_pPosition;
}

void CNinePatch::Set_Position(D3DXVECTOR2 value)
{
	this->m_pPosition->x = value.x;
	this->m_pPosition->y = value.y;
	D3DXMatrixTranslation(this->m_pWorld, value.x, 0, value.y);
}

D3DXVECTOR2 CNinePatch::Get_Size()
{
	return *this->m_pSize;
}

void CNinePatch::Set_Size(D3DXVECTOR2 value)
{
	this->m_pSize->x = value.x;
	this->m_pSize->y = value.y;
}

CTexture2D* CNinePatch::Get_Texture()
{
	return this->m_pTexture;
}

void CNinePatch::Set_Texture(CTexture2D* value, D3DXVECTOR2 size)
{
	this->m_pTexture = value;
	this->m_pTextureSize->x = size.x;
	this->m_pTextureSize->y = size.y;
}
#pragma endregion

#pragma region Constructors and Finalizers
CNinePatch::CNinePatch(CGame* game)
: CDrawableGameComponent(game)
{
	CNinePatch::g_iReferences++;

	this->m_pTexture = NULL;
	this->m_pTextureSize = new D3DXVECTOR2(0,0);
	this->m_pPosition = new D3DXVECTOR2(0,0);
	this->m_pSize = new D3DXVECTOR2(1,1);
	D3DXMatrixTranslation((this->m_pWorld = new D3DXMATRIX()), this->m_pPosition->x, 0, this->m_pPosition->y);
}

CNinePatch::CNinePatch(CGame* game, D3DXVECTOR2 position, D3DXVECTOR2 size, CTexture2D* texture, D3DXVECTOR2 textureSize)
:CDrawableGameComponent(game)
{
	CNinePatch::g_iReferences++;

	this->m_pTexture = texture;
	this->m_pTextureSize = new D3DXVECTOR2(textureSize.x,textureSize.y);
	this->m_pPosition = new D3DXVECTOR2(position.x, position.y);
	this->m_pSize = new D3DXVECTOR2(size.x, size.y);
	D3DXMatrixTranslation((this->m_pWorld = new D3DXMATRIX()), this->m_pPosition->x, 0, this->m_pPosition->y);
}

CNinePatch::~CNinePatch()
{
	CNinePatch::g_iReferences--;

	//if there are no more enemy unit references, de-reference the global variables
	if(CNinePatch::g_iReferences == 0)
	{
		delete CNinePatch::g_pMesh;
		CNinePatch::g_pMesh = NULL;

		delete CNinePatch::g_pEffect;
		CNinePatch::g_pEffect = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;

	if(this->m_pPosition != NULL)
		delete this->m_pPosition;

	if(this->m_pSize != NULL)
		delete this->m_pSize;

	if(this->m_pTextureSize != NULL)
		delete this->m_pTextureSize;
}
#pragma endregion

#pragma region static Methods
/**
 * Builds the static mesh for the Enemy unit class
 *
 * @param	device	The graphics device that is registered to the game
 */
void CNinePatch::BuildMesh(CGraphicsDevice* device)
{
	if(CNinePatch::g_pMesh != NULL)
		delete CNinePatch::g_pMesh;
	CNinePatch::g_pMesh = new CMesh(device);

	NinePatchVertex vertices[16];
	float scale = 1.0f / 3.0f;
	for(int i = 0; i < 4; i++)
	{
		float y = i * scale;
		for(int j = 0; j < 4; j++)
		{
			float x = j * scale;
			
			int index = i * 4 + j;
			vertices[index].Position.x = x;
			vertices[index].Position.y = 0;
			vertices[index].Position.z = y;
			vertices[index].Index = index + 1;
			vertices[index].TextureCoord.x = x;
			vertices[index].TextureCoord.y = y;
		}
	}
	CNinePatch::g_pMesh->SetVertices((void*)vertices, 16);

	UINT indices[54];
	int k = 0;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			indices[k++] = i*4 + j;
			indices[k++] = (i + 1)*4 + j;
			indices[k++] = i*4 + j + 1;

			indices[k++] = i*4 + j + 1;
			indices[k++] = (i + 1)*4 + j;
			indices[k++] = (i + 1)*4 + j + 1;
		}
	}
	CNinePatch::g_pMesh->SetIndices((UINT*)indices, 54);

	CNinePatch::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)NinePatchVertex::VertexElements, NinePatchVertex::VertexElementCount);
}
#pragma endregion


#pragma region Overriden Methods
void CNinePatch::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	//If the global variables are not set, load the content associated to them
	if(CNinePatch::g_pCamera == NULL)
		CNinePatch::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	
	if(CNinePatch::g_pEffect == NULL)
		CNinePatch::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\NinePatchEffect.fx");
	if(CNinePatch::g_pMesh == NULL)
		CNinePatch::BuildMesh(this->Get_Game()->Get_GraphicsDevice());
}

void CNinePatch::Draw(CGameTime* gameTime)
{
	//Call CGameUnit's draw
	CDrawableGameComponent::Draw(gameTime);

	this->Get_Game()->Get_GraphicsDevice()->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)NinePatchVertex::VertexElements, NinePatchVertex::VertexElementCount);

	//Update the effect's parameters
	CNinePatch::g_pEffect->Set_View(&CNinePatch::g_pCamera->Get_View());
	CNinePatch::g_pEffect->Set_World(this->m_pWorld);
	CNinePatch::g_pEffect->Set_Projection(&CNinePatch::g_pCamera->Get_Projection());
	if(this->m_pTexture != NULL)
		CNinePatch::g_pEffect->SetVariable("InputTexture", this->m_pTexture);

	CNinePatch::g_pEffect->SetVariable("Size", *this->m_pSize);
	CNinePatch::g_pEffect->SetVariable("TextureSize", *this->m_pTextureSize);
	
	//Run through the effect's draw sequence
	CNinePatch::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CNinePatch::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CNinePatch::g_pMesh->Draw();
		}
	}
	CNinePatch::g_pEffect->End();
}
#pragma endregion


#pragma region Nested Classes
const D3D10_INPUT_ELEMENT_DESC NinePatchVertex::VertexElements[] = 
{
	{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",		0, DXGI_FORMAT_R32_UINT,		0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0}
};

const int NinePatchVertex::VertexElementCount = 3;

NinePatchVertex::NinePatchVertex()
{
	this->Position.x = 0;
	this->Position.y = 0;
	this->Position.z = 0;
	this->Index = 0;
	this->TextureCoord.x = 0;
	this->TextureCoord.y = 0;
}
#pragma endregion