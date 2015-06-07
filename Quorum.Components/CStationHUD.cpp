#include "Quorum.Components.h"

#pragma region Static Variables
int CStationHUD::g_iReferences = 0;
CMesh* CStationHUD::g_pMeshGear = NULL;
CMesh* CStationHUD::g_pMeshAmmo = NULL;
CMesh* CStationHUD::g_pMeshHealth = NULL;

CTexture2D* CStationHUD::g_pTextureHUDGear = NULL;
CTexture2D* CStationHUD::g_pTextureAmmo = NULL;
CTexture2D* CStationHUD::g_pTextureAmmoMask = NULL;
CTexture2D* CStationHUD::g_pTextureHealth = NULL;
CTexture2D* CStationHUD::g_pTextureHealthMask = NULL;

CEffect* CStationHUD::g_pEffect = NULL;
ICameraService* CStationHUD::g_pCamera = NULL;
#pragma endregion

#pragma region Constructors and Deconstructors
CStationHUD::CStationHUD(CGame* game, CStation* station)
	:CDrawableGameComponent(game)
{
	g_iReferences++;

	this->m_pStation = station;

	// Setup the Gear transforms
	D3DXMatrixIdentity(this->m_pGearWorld = new D3DXMATRIX());

	D3DXMatrixIdentity(this->m_pGearTranslation = new D3DXMATRIX());
	D3DXMATRIX gearTransTmp;
	D3DXMatrixTranslation(&gearTransTmp, 0.7f, 0, 0);
	D3DXMatrixMultiply(this->m_pGearTranslation, this->m_pGearTranslation, &gearTransTmp);

	D3DXMatrixIdentity(this->m_pGearRotation = new D3DXMATRIX());

	// Setup the Ammo transforms
	D3DXMatrixIdentity(this->m_pAmmoWorld = new D3DXMATRIX());

	D3DXMatrixIdentity(this->m_pAmmoTranslation = new D3DXMATRIX());
	D3DXMATRIX ammoTransTmp;
	D3DXMatrixTranslation(&ammoTransTmp, 3.5f, 0, -1.9f);
	D3DXMatrixMultiply(this->m_pAmmoTranslation, this->m_pAmmoTranslation, &ammoTransTmp);

	// Setup the Health transforms
	D3DXMatrixIdentity(this->m_pHealthWorld = new D3DXMATRIX());

	D3DXMatrixIdentity(this->m_pHealthTranslation = new D3DXMATRIX());
	D3DXMATRIX healthTransTmp;
	D3DXMatrixTranslation(&healthTransTmp, 3.0f, 0, 2.0f);
	D3DXMatrixMultiply(this->m_pHealthTranslation, this->m_pHealthTranslation, &healthTransTmp);

	// Setup the Reverse Station Rotation transform
	D3DXMatrixIdentity(this->m_pStationOffsetRotation = new D3DXMATRIX());
	D3DXMatrixInverse(this->m_pStationOffsetRotation, NULL, this->m_pStation->m_pRotation);
}

CStationHUD::~CStationHUD()
{
	CStationHUD::g_iReferences--;

	//If there are no more references to this type of object, de-reference the global variables
	if(CStationHUD::g_iReferences == 0)
	{
		delete CStationHUD::g_pMeshGear;
		CStationHUD::g_pMeshGear = NULL;

		delete CStationHUD::g_pMeshAmmo;
		CStationHUD::g_pMeshAmmo = NULL;

		delete CStationHUD::g_pMeshHealth;
		CStationHUD::g_pMeshHealth = NULL;

		delete CStationHUD::g_pEffect;
		CStationHUD::g_pEffect = NULL;
		
		delete CStationHUD::g_pTextureHUDGear;
		CStationHUD::g_pTextureHUDGear = NULL;

		delete CStationHUD::g_pTextureAmmo;
		CStationHUD::g_pTextureAmmo = NULL;

		delete CStationHUD::g_pTextureAmmoMask;
		CStationHUD::g_pTextureAmmoMask = NULL;

		delete CStationHUD::g_pTextureHealth;
		CStationHUD::g_pTextureHealth = NULL;
		
		delete CStationHUD::g_pTextureHealthMask;
		CStationHUD::g_pTextureHealthMask = NULL;

		//delete CStation::g_pActiveTexture;
		//CStation::g_pActiveTexture = NULL;
	}

	//Delete the world transforms
	if(this->m_pGearWorld != NULL)
		delete this->m_pGearWorld;
	if(this->m_pGearTranslation != NULL)
		delete this->m_pGearTranslation;
	if(this->m_pGearRotation != NULL)
		delete this->m_pGearRotation;

	if(this->m_pStationOffsetRotation != NULL)
		delete this->m_pStationOffsetRotation;

	if(this->m_pAmmoWorld != NULL)
		delete this->m_pAmmoWorld;
	if(this->m_pAmmoTranslation != NULL)
		delete this->m_pAmmoTranslation;


	if(this->m_pHealthWorld != NULL)
		delete this->m_pHealthWorld;
	if(this->m_pHealthTranslation != NULL)
		delete this->m_pHealthTranslation;
}

#pragma endregion

#pragma region Methods
void CStationHUD::UpdateHealth(CGameTime* gameTime)
{
	D3DXMATRIX tempShipRotation;
	D3DXMatrixInverse(&tempShipRotation, NULL, this->m_pStation->m_pShip->m_pRotation);

	// Now let's rotate it against the ship
	D3DXMatrixMultiply(this->m_pHealthWorld, this->m_pStationOffsetRotation, &tempShipRotation);

	// And then move us out a little bit
	D3DXMatrixMultiply(this->m_pHealthWorld, this->m_pHealthWorld, this->m_pHealthTranslation);

	// Then add my Station's transform
	D3DXMatrixMultiply(this->m_pHealthWorld, this->m_pHealthWorld, this->m_pStation->Get_Transform());
}

void CStationHUD::UpdateAmmo(CGameTime* gameTime)
{
	D3DXMATRIX tempShipRotation;
	D3DXMatrixInverse(&tempShipRotation, NULL, this->m_pStation->m_pShip->m_pRotation);

	// Now let's rotate it against the ship
	D3DXMatrixMultiply(this->m_pAmmoWorld, this->m_pStationOffsetRotation, &tempShipRotation);

	// And then move us out a little bit
	D3DXMatrixMultiply(this->m_pAmmoWorld, this->m_pAmmoWorld, this->m_pAmmoTranslation);

	// Then add my Station's transform
	D3DXMatrixMultiply(this->m_pAmmoWorld, this->m_pAmmoWorld, this->m_pStation->Get_Transform());
}

void CStationHUD::UpdateGear(CGameTime* gameTime)
{
	// Reset our transformation to the origin
	D3DXMatrixIdentity(this->m_pGearWorld);

	// Now let's rotate it against the ship
	D3DXMATRIX tempRotation;
	D3DXMatrixRotationAxis(&tempRotation, &D3DXVECTOR3(0,1,0), 4.0f * this->m_pStation->Get_Ship()->Get_Rotation());
	D3DXMatrixIdentity(this->m_pGearRotation);
	D3DXMatrixMultiply(this->m_pGearRotation, this->m_pGearRotation, &tempRotation);
	D3DXMatrixMultiply(this->m_pGearWorld, this->m_pGearWorld, this->m_pGearRotation);

	// And then move us out a little bit
	D3DXMatrixMultiply(this->m_pGearWorld, this->m_pGearWorld, this->m_pGearTranslation);

	// Then add my Station's transform
	D3DXMatrixMultiply(this->m_pGearWorld, this->m_pGearWorld, this->m_pStation->Get_Transform());
}

void CStationHUD::DrawHealth(CGraphicsDevice* device, CGameTime* gameTime)
{
	// FOR THE HEALTH
	CStationHUD::g_pEffect->Set_World(this->m_pHealthWorld);
	if(CStationHUD::g_pTextureAmmo != NULL)
		CStationHUD::g_pEffect->SetVariable("InputTexture", CStationHUD::g_pTextureHealth);

	if(CStationHUD::g_pTextureHUDGear != NULL)
		CStationHUD::g_pEffect->SetVariable("GradientTexture", CStationHUD::g_pTextureHealthMask);

	CStationHUD::g_pEffect->Set_Technique("AlphaHud");
	CStationHUD::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CStationHUD::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);

		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CStationHUD::g_pMeshHealth->Draw();
		}
	}
	CStationHUD::g_pEffect->End();
}

void CStationHUD::DrawAmmo(CGraphicsDevice* device, CGameTime* gameTime)
{
	// FOR THE AMMO
	CStationHUD::g_pEffect->Set_World(this->m_pAmmoWorld);
	if(CStationHUD::g_pTextureAmmo != NULL)
		CStationHUD::g_pEffect->SetVariable("InputTexture", CStationHUD::g_pTextureAmmo);

	if(CStationHUD::g_pTextureHUDGear != NULL)
		CStationHUD::g_pEffect->SetVariable("GradientTexture", CStationHUD::g_pTextureAmmoMask);

	CStationHUD::g_pEffect->SetVariable("Opacity", 0.85f);

	CStationHUD::g_pEffect->Set_Technique("AlphaHud");
	CStationHUD::g_pEffect->Begin();
	{
		
		ID3D10EffectTechnique* technique = CStationHUD::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);

		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CStationHUD::g_pMeshAmmo->Draw();
		}
	}
	CStationHUD::g_pEffect->End();

	// FOR THE AMMO LCD
	CLCDisplay::DrawLCD(this->Get_Game(), device, this->m_pAmmoWorld, this->m_pStation->m_sAttributes.Health, this->m_pStation->Get_Turret()->Get_Type());
}

void CStationHUD::DrawGear(CGraphicsDevice* device, CGameTime* gameTime)
{
	CStationHUD::g_pEffect->Set_View(&CStationHUD::g_pCamera->Get_View());
	CStationHUD::g_pEffect->Set_World(this->m_pGearWorld);
	CStationHUD::g_pEffect->Set_Projection(&CStationHUD::g_pCamera->Get_Projection());

	CStationHUD::g_pEffect->SetVariable("StationType", this->m_pStation->Get_Turret()->Get_Type());
	CStationHUD::g_pEffect->SetVariable("StationHealth", this->m_pStation->m_sAttributes.Health);
	CStationHUD::g_pEffect->SetVariable("Opacity", 0.25f);

	if(CStationHUD::g_pTextureHUDGear != NULL)
		CStationHUD::g_pEffect->SetVariable("InputTexture", CStationHUD::g_pTextureHUDGear);

	
	// FOR THE GEAR
	CStationHUD::g_pEffect->Set_Technique("StandardHud");
	CStationHUD::g_pEffect->Begin();
	{
		
		ID3D10EffectTechnique* technique = CStationHUD::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);

		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CStationHUD::g_pMeshGear->Draw();
		}
	}
	CStationHUD::g_pEffect->End();
}
#pragma endregion

#pragma region Static Methods
//Build the static mesh for the ship
CMesh* CStationHUD::BuildMesh(CGraphicsDevice* device, float scale)
{
	//float scale = 4.9f;
	CMesh* mesh = new CMesh(device);
	
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -scale;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scale;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = scale;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scale;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -scale;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scale;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = scale;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scale;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	mesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	mesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	mesh->SetAttributes((UINT*)attributes, 2);
	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	return mesh;
}
#pragma endregion

#pragma region Overriden Methods
//Load the content related to the ship
void CStationHUD::LoadContent()
{
	CDrawableGameComponent::LoadContent();
	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	//Load the static camera service
	if(CStationHUD::g_pCamera == NULL)
		CStationHUD::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	
	//Create the mesh objects
	if(CStationHUD::g_pMeshGear == NULL)
		CStationHUD::g_pMeshGear = CStationHUD::BuildMesh(device,2.2f);
	
	if(CStationHUD::g_pMeshAmmo == NULL)
		CStationHUD::g_pMeshAmmo = CStationHUD::BuildMesh(device,2.2f);

	if(CStationHUD::g_pMeshHealth == NULL)
		CStationHUD::g_pMeshHealth = CStationHUD::BuildMesh(device,1.1f);

	//Load the effect file
	if(CStationHUD::g_pEffect == NULL)
		CStationHUD::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\HudEffect.fx");

	//Create the textures
	if(CStationHUD::g_pTextureHUDGear == NULL)
		CStationHUD::g_pTextureHUDGear = CTexture2D::Load(device, "..\\Assets\\Art\\innerGear.png");

	if(CStationHUD::g_pTextureAmmo == NULL)
		CStationHUD::g_pTextureAmmo = CTexture2D::Load(device, "..\\Assets\\Art\\ammoGuage.png");

	if(CStationHUD::g_pTextureAmmoMask == NULL)
		CStationHUD::g_pTextureAmmoMask = CTexture2D::Load(device, "..\\Assets\\Art\\ammoGuage_mask.png");

	if(CStationHUD::g_pTextureHealth == NULL)
		CStationHUD::g_pTextureHealth = CTexture2D::Load(device, "..\\Assets\\Art\\healthGuage.png");

	if(CStationHUD::g_pTextureHealthMask == NULL)
		CStationHUD::g_pTextureHealthMask = CTexture2D::Load(device, "..\\Assets\\Art\\healthGuage_mask.png");
}

//Update the ship's world transform
void CStationHUD::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	this->UpdateGear(gameTime);
	this->UpdateAmmo(gameTime);
	this->UpdateHealth(gameTime);
}

//Draw the ship
void CStationHUD::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	this->DrawGear(device, gameTime);
	this->DrawAmmo(device, gameTime);
	this->DrawHealth(device, gameTime);
}
#pragma endregion