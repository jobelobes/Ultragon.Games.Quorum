/**
 * @description 
 *   Class for the targetting reticule for the turrets. The reticule will be allowed to roam around in 
 *	  freespace while the turret follows it around
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CTarget::g_iReferences = 0;
CMesh* CTarget::g_pMesh = NULL;
CEffect* CTarget::g_pEffect = NULL;
CTexture2D* CTarget::g_pTexture = NULL;

ICameraService* CTarget::g_pCamera = NULL;

const float CTarget::InputMoveRate = 20;
const float CTarget::InputRotateRate = D3DX_PI / 4;
#pragma endregion

#pragma region Constructors and Deconstructors
//Default Constructor
CTarget::CTarget(CGame* game, D3DXMATRIX translation, D3DXMATRIX rotation, D3DXMATRIX* shipTransform)
	:CDrawableGameComponent(game)
{
	//Declare the variables
	this->m_bIsTurretBound = false;

	this->m_pPosition = new D3DXVECTOR2(translation._41,translation._43);

	this->m_pTranslation = new D3DXMATRIX(*translation.m);
	this->m_pRotation = new D3DXMATRIX(*rotation.m);
	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());

	this->m_pShipTransform = shipTransform;
}
#pragma endregion

#pragma region Properties
//returns the world transform for this target
D3DXMATRIX* CTarget::Get_Transform()
{
	return this->m_pWorld;
}

//Returns whether or not the target is bound to a turret station
bool CTarget::Get_IsTurretBound()
{
	return this->m_bIsTurretBound;
}

//Sets the IsTurretBound flag to indicate whether or not the target will be drawn
void CTarget::Set_IsTurretBound(bool value)
{
	this->m_bIsTurretBound = value;
}
#pragma endregion

#pragma region IBasicUnit Properties
//Get the movement rate of the target
float CTarget::Get_MoveRate()
{
	return CTarget::InputMoveRate;
}
#pragma endregion

#pragma region Static Methods
//Build the static mesh
void CTarget::BuildMesh(CGraphicsDevice* device)
{
	// delete any old references
	if(CTarget::g_pMesh != NULL)
		delete CTarget::g_pMesh;

	//Create the mesh object
	CTarget::g_pMesh = new CMesh(device);
	float scale = 1.0f;

	//Create the vertices and texture coordinates
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.5f * scale;
	vertices[0].Position.y = 0 * scale;
	vertices[0].Position.z = -0.5f * scale;
	/*vertices[0].Normal.x = 0;
	vertices[0].Normal.y = 1;
	vertices[0].Normal.z = 0;*/
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 0.5f * scale;
	vertices[1].Position.y = 0 * scale;
	vertices[1].Position.z = -0.5f * scale;
	/*vertices[1].Normal.x = 0;
	vertices[1].Normal.y = 1;
	vertices[1].Normal.z = 0;*/
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -0.5f * scale;
	vertices[2].Position.y = 0 * scale;
	vertices[2].Position.z = 0.5f * scale;
	/*vertices[2].Normal.x = 0;
	vertices[2].Normal.y = 1;
	vertices[2].Normal.z = 0;*/
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 0.5f * scale;
	vertices[3].Position.y = 0 * scale;
	vertices[3].Position.z = 0.5f * scale;
	/*vertices[3].Normal.x = 0;
	vertices[3].Normal.y = 1;
	vertices[3].Normal.z = 0;*/
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	CTarget::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CTarget::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CTarget::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CTarget::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion

#pragma region IBasicUnit Methods
//Move the target based on an offset
void CTarget::Move(D3DXVECTOR2 offset)
{
	//Adjust the translation matrix based on the offset
	(*this->m_pPosition) += offset;
	::D3DXMatrixTranslation(this->m_pTranslation, this->m_pPosition->x, 0, this->m_pPosition->y);
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the target
void CTarget::Initialize()
{
	//Call CDrawableGameComponent's initialize
	CDrawableGameComponent::Initialize();
}

//Load the content associated to a target
void CTarget::LoadContent()
{
	//Call CDrawableGameComponent's load content
	CDrawableGameComponent::LoadContent();

	//Load the camera service if the global is null
	if(CTarget::g_pCamera == NULL)
	{
		CTarget::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
		if(CTarget::g_pCamera == NULL)
			exit(-1);
	}

	//Load the texture, effect, and build the mesh
	if(CTarget::g_pEffect == NULL)
		CTarget::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultTextureEffect.fx");
	if(CTarget::g_pTexture == NULL)
		CTarget::g_pTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\target_0.png");
	if(CTarget::g_pMesh == NULL)
		CTarget::BuildMesh(this->Get_Game()->Get_GraphicsDevice());
}

//Update the target's world transform
void CTarget::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
}

//Draw the target based on the world transform
void CTarget::Draw(CGameTime* gameTime)
{
	if(this->m_bIsTurretBound)
	{
		CDrawableGameComponent::Draw(gameTime);

		CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
		
		device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

		CTarget::g_pEffect->Set_View(&CTarget::g_pCamera->Get_View());
		CTarget::g_pEffect->Set_World(this->m_pWorld);
		CTarget::g_pEffect->Set_Projection(&CTarget::g_pCamera->Get_Projection());
		if(CTarget::g_pTexture != NULL)
			CTarget::g_pEffect->SetVariable("InputTexture", CTarget::g_pTexture);
		CTarget::g_pEffect->Begin();
		{
			ID3D10EffectTechnique* technique = CTarget::g_pEffect->Get_CurrentTechnique();

			D3D10_TECHNIQUE_DESC td;
			technique->GetDesc(&td);
			for( UINT p = 0; p < td.Passes; ++p )
			{
				technique->GetPassByIndex(p)->Apply(0);
				CTarget::g_pMesh->Draw();
			}
		}
		CTarget::g_pEffect->End();
	}
}
#pragma endregion

