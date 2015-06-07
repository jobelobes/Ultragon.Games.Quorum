/**
 * @description 
 *   Since the ship is composed of stations, this class implements the basic needs
 *   of a single station.  On the ship object, this station is rotated around the center
 *   to create the hull.  This object is also the parent object of the turret.  The station
 *   type defined whether the station can maintain a turret and what type of turret.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CStation::g_iReferences = 0;
int CStation::g_iStationCount = 3;

CMesh* CStation::g_pMesh = NULL;
CEffect* CStation::g_pEffect = NULL;
CTexture2D* CStation::g_pSegmentTexture = NULL;
CTexture2D* CStation::g_pActiveTexture = NULL;
CTexture2D** CStation::g_pStationTexture = NULL;

ICameraService* CStation::g_pCamera = NULL;
ICollisionService* CStation::g_pCollision = NULL;
ISceneManager* CStation::g_pSceneManager = NULL;
#pragma endregion

#pragma region Properties
//Return the station's world transform
//I've been breaking here a lot, too.
D3DXMATRIX* CStation::Get_Transform()
{
	return this->m_pWorld;
}

//Return the turret associated with the station (if any)
CTurret* CStation::Get_Turret()
{
	if(this->m_sAttributes.Type != StationType::Pilot)
		return this->m_pTurret;
	return NULL;
}

//Return the station's type
StationType CStation::Get_StationType()
{
	return this->m_sAttributes.Type;
}

//Return whether or not the station is bound to a controller
bool CStation::Get_IsBound()
{
	return this->m_bIsBound;
}

//Set the station's bound flag
void CStation::Set_IsBound(bool value)
{
	this->m_bIsBound = value;
}

//Return the station's move rate (if there is one)
int CStation::Get_MoveRate()
{
	if(this->m_sAttributes.Type == StationType::Pilot)
		return this->m_pShip->Get_MoveRate();
	
	return NULL;
}

//Return the station's rotation rate
float CStation::Get_RotateRate()
{
	if(this->m_sAttributes.Type == StationType::Turret)
		return this->m_pTurret->Get_RotateRate();

	return this->m_pShip->Get_RotateRate();
}

CShip* CStation::Get_Ship()
{
	return this->m_pShip;
}

void CStation::Set_HUDEnabled(bool value)
{
	if(this->m_pHUD != NULL)
	{
		this->m_pHUD->Set_Enabled(value);
		this->m_pHUD->Set_Visible(value);
	}
}
#pragma endregion

#pragma region Constructors and Finalizers
//Parameterized constructor that tatkes in a ship, type, and custom translation and rotation matrices
CStation::CStation(CGame* game, CShip* ship, D3DXMATRIX translate, D3DXMATRIX rotation, StationAttributes attributes)
: CDrawableGameComponent(game)
{
	//Increment the references
	CStation::g_iReferences++;

	//Declare the variables
	this->m_pShip = ship;
	this->m_pTurret = NULL;
	this->m_pHUD = NULL;

	memcpy(&this->m_sAttributes, &attributes, sizeof(StationAttributes));

	this->m_bIsBound = false;

	this->m_pTranslation = new D3DXMATRIX(*translate.m);
	this->m_pRotation = new D3DXMATRIX(*rotation.m);
	this->m_pWorld = new D3DXMATRIX();

	this->m_sAttributes.Health = 100;
}

//Deconstructor
CStation::~CStation()
{
	//De-increment the references
	CStation::g_iReferences--;

	//If there are no more references to this type of object, de-reference the global variables
	if(CStation::g_iReferences == 0)
	{
		delete CStation::g_pMesh;
		CStation::g_pMesh = NULL;

		delete CStation::g_pEffect;
		CStation::g_pEffect = NULL;
		
		delete CStation::g_pSegmentTexture;
		CStation::g_pSegmentTexture = NULL;
		
		delete CStation::g_pActiveTexture;
		CStation::g_pActiveTexture = NULL;
	}

	//Delete the world transforms
	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
	if(this->m_pTranslation != NULL)
		delete this->m_pTranslation;
	if(this->m_pRotation != NULL)
		delete this->m_pRotation;
}
#pragma endregion

#pragma region Static Methods
//Load an array of textures associated to the object. Textures are directly related to the types of stations that exist
CTexture2D** CStation::LoadTextures(CGraphicsDevice* device, int size)
{
	char path[256];
	CTexture2D** textures = new CTexture2D*[size];
	for(int i = 0; i < size; i++)
	{
		sprintf(&(path[0]), "..\\Assets\\Art\\station_%i.png", i);
		textures[i] = CTexture2D::Load(device, &(path[0]));
	}
	return textures;
}

//Build a turret object
void CStation::BuildTurret()
{
	std::vector<TURRET*> turrets;
	((CQuorumSceneManager*)CShip::g_pSceneManager)->Get_GameContent()->Get_Turrets(turrets);

	D3DXMATRIX translation;
	D3DXMATRIX rotation;

	::D3DXMatrixTranslation(&translation, 0.95f, 0, -0.03f);
	::D3DXMatrixIdentity(&rotation);
	
	TurretAttributes tmp;

	//load the attributes for the turret
	for(int i = 0; i < turrets.size(); i++)
	{
		//Only create an attributes struct if the turret type requested by the station matches one in the loaded content
		if(turrets[i]->type == this->m_sAttributes.TurretType)
		{
			tmp.Name = turrets[i]->name;
			tmp.Type = turrets[i]->type;
			tmp.Cooldown = turrets[i]->cooldown;
			tmp.Speed = turrets[i]->speed;
			tmp.Damage = turrets[i]->damage;
			tmp.Lifespan = turrets[i]->lifespan;
		}
	}
	
	this->m_pTurret = new CTurret(this->Get_Game(), this, translation, rotation, tmp);
	this->Get_Game()->Get_Components()->Add(this->m_pTurret);
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the station object
void CStation::Initialize()
{
	//Call CDrawableGameComponent's initialize
	CDrawableGameComponent::Initialize();

	if(this->m_pTurret != NULL)
		this->m_pTurret->Initialize();
}

//Load content associated to the stations
void CStation::LoadContent()
{
	//Call CDrawableGameComponent's load content
	CDrawableGameComponent::LoadContent();

	//Grab the game's graphics device
	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	//Grab the camera service if the global is null
	if(CStation::g_pCamera == NULL)
		CStation::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	
	//Build the static mesh
	if(CStation::g_pMesh == NULL)
		CStation::BuildMesh(device);
	
	//Load the static effect
	if(CStation::g_pEffect == NULL)
		CStation::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\StationEffect.fx");

	//Load the static ship segment texture
	if(CStation::g_pSegmentTexture == NULL)
		CStation::g_pSegmentTexture = CTexture2D::Load(device, "..\\Assets\\Art\\hull_animation.png");
	
	//Load the texture associated to when a crew member is bound to that station
	if(CStation::g_pActiveTexture == NULL)
		CStation::g_pActiveTexture = CTexture2D::Load(device, "..\\Assets\\Art\\hull_stripe.png");	

	//Grab the station texture
	if(CStation::g_pStationTexture == NULL)
		CStation::g_pStationTexture = CStation::LoadTextures(this->Get_Game()->Get_GraphicsDevice(), CStation::g_iStationCount);

	if(this->m_sAttributes.Type != StationType::Pilot)
	{
		this->BuildTurret();

		this->m_pHUD = new CStationHUD(this->Get_Game(), this);
		this->m_pHUD->Set_Enabled(false);
		this->m_pHUD->Set_Visible(false);
		this->Get_Game()->Get_Components()->Add(this->m_pHUD);
	}
}

//Update the station's world transform
void CStation::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	// update the segment's position
	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pShip->Get_Transform());
}

//Draw the station
void CStation::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	// Don't tell me what I can and cannot do! :-P
	// Now it's there, and there's nothing you can do about it. :-P
	/*if(this->m_sAttributes->Type != StationType::Pilot)
		CFloorHealth::DrawFloor(this->Get_Game(), device, this->m_pWorld, 100);*/

	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);


	CStation::g_pEffect->Set_View(&CStation::g_pCamera->Get_View());
	CStation::g_pEffect->Set_World(this->m_pWorld);
	CStation::g_pEffect->Set_Projection(&CStation::g_pCamera->Get_Projection());

	if(this->m_sAttributes.Type != StationType::Pilot)
	{
		CStation::g_pEffect->SetVariable("ActiveColor", D3DXCOLOR(1,0,0,this->Get_Turret()->Get_IsBound()?1:0));
		if(CStation::g_pSegmentTexture != NULL)
			CStation::g_pEffect->SetVariable("SegmentTexture", CStation::g_pSegmentTexture);
		if(CStation::g_pActiveTexture != NULL)
			CStation::g_pEffect->SetVariable("ActiveTexture", CStation::g_pActiveTexture);
	}

	if(CStation::g_pStationTexture != NULL)
		CStation::g_pEffect->SetVariable("StationTexture", CStation::g_pStationTexture[this->m_sAttributes.Type]);

	CStation::g_pEffect->SetVariable("StationHealth", this->m_sAttributes.Health);
	CStation::g_pEffect->SetVariable("StationType", (int)this->m_sAttributes.Type);
	CStation::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CStation::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CStation::g_pMesh->Draw();
		}
	}
	CStation::g_pEffect->End();
}
#pragma endregion

#pragma region Static Methods

//Build the static mesh
void CStation::BuildMesh(CGraphicsDevice* device)
{
	// delete any old references
	if(CStation::g_pMesh != NULL)
		delete CStation::g_pMesh;

	float scale = 5.0f;
	CStation::g_pMesh = new CMesh(device);
	
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.25f * scale;
	vertices[0].Position.y = 0 * scale;
	vertices[0].Position.z = -0.5f * scale;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 0.25f * scale;
	vertices[1].Position.y = 0 * scale;
	vertices[1].Position.z = -0.5f * scale;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -0.25f * scale;
	vertices[2].Position.y = 0 * scale;
	vertices[2].Position.z = 0.5f * scale;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 0.25f * scale;
	vertices[3].Position.y = 0 * scale;
	vertices[3].Position.z = 0.5f * scale;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	CStation::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CStation::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CStation::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CStation::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion
