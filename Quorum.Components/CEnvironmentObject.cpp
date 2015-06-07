/**
 * @description 
 *   Basic bullet; maintains the global mesh and a list of textures(for each type of bullet)
 *   and draw its position in relation to its given position and initial velocity.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CEnvironmentObject::g_iReferences = 0;
int CEnvironmentObject::g_iTextureCount = 3;
int CEnvironmentObject::g_iNebulaCount = 8;

CEffect* CEnvironmentObject::g_pEffect = NULL;
CMesh** CEnvironmentObject::g_pMeshes = NULL;
CTexture2D** CEnvironmentObject::g_pPlanetTextures = NULL;
CTexture2D** CEnvironmentObject::g_pNebulaTextures = NULL;
CTexture2D** CEnvironmentObject::g_pTextures = NULL;

ICameraService* CEnvironmentObject::g_pCamera = NULL;
ICollisionService* CEnvironmentObject::g_pCollision = NULL;
IWorldScroller* CEnvironmentObject::g_pWorldScroller = NULL;
ISceneManager* CEnvironmentObject::g_pSceneManager= NULL;
#pragma endregion

#pragma region Properties
//Return the position
D3DXVECTOR2 CEnvironmentObject::Get_Position()
{
	return this->m_sPosition;
}

//Set the position
void CEnvironmentObject::Set_Position(D3DXVECTOR2 value)
{
	memcpy(&this->m_sPosition, &value, sizeof(D3DXVECTOR2));

	//adjust the bounding box 
	D3DXVECTOR2 min = this->m_sPosition + this->m_pScale * -0.5;
	D3DXVECTOR2 max = this->m_sPosition + this->m_pScale * 0.5;
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(min.x, 0, min.y), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(max.x, 0, max.y), sizeof(D3DXVECTOR3));
}

float CEnvironmentObject::Get_Rotation()
{
	return this->m_fRotation;
}

void CEnvironmentObject::Set_Rotation(float value)
{
	this->m_fRotation = value;

	D3DXMatrixRotationY(this->m_pRotation, this->m_fRotation);
}

CMesh* CEnvironmentObject::Get_Mesh(EnvironmentObjectType type)
{
	if(CEnvironmentObject::g_pMeshes != NULL)
		return CEnvironmentObject::g_pMeshes[type];
	return NULL;
}

CTexture2D* CEnvironmentObject::Get_Texture(EnvironmentObjectType type)
{
	if(CEnvironmentObject::g_pTextures != NULL)
		return CEnvironmentObject::g_pTextures[type];
	return NULL;
}

//Return the scale of the image
D3DXVECTOR2 CEnvironmentObject::Get_Scale()
{
	return this->m_pScale;
}

//Set the scale for the animation
void CEnvironmentObject::Set_Scale(D3DXVECTOR2 value)
{
	this->m_pScale = value;
	if(this->m_iObjectType != EnvironmentObjectType::Planet)
	{
		this->m_pAnimation->Set_ImageScale(D3DXVECTOR2(this->m_pScale.x, this->m_pScale.y));

		//adjust the bounding box 
		D3DXVECTOR2 min = this->m_sPosition + this->m_pScale * -0.5;
		D3DXVECTOR2 max = this->m_sPosition + this->m_pScale * 0.5;
		CBoundingBox* bbox = this->Get_BoundingBox();
		memcpy(&bbox->Min, &D3DXVECTOR3(min.x, 0, min.y), sizeof(D3DXVECTOR3));
		memcpy(&bbox->Max, &D3DXVECTOR3(max.x, 0, max.y), sizeof(D3DXVECTOR3));
	}
}

CAnimation* CEnvironmentObject::Get_Animation()
{
	if(this->m_pAnimation != NULL)
		return this->m_pAnimation;

	return NULL;
}

//return the object type
EnvironmentObjectType CEnvironmentObject::Get_ObjectType()
{
	return this->m_iObjectType;
}

//Set the object type
void CEnvironmentObject::Set_ObjectType(EnvironmentObjectType type)
{
	this->m_iObjectType = type;
}

#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CEnvironmentObject::CEnvironmentObject(CGame* game, EnvironmentObjectType type)
: CGameUnit(game, CQuorumIdentifiers::Quorum_EnvObject) 
{
	//Declare the variables
	CEnvironmentObject::g_iReferences++;

	ZeroMemory(&this->m_sPosition, sizeof(D3DXVECTOR2));
	this->m_fRotation = 0.0f;

	this->m_iObjectType = type;
	this->m_pAnimation = NULL;

	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pScaling = new D3DXMATRIX());

	ZeroMemory(&this->m_sPosition, sizeof(D3DXVECTOR2));

	this->m_pScale = D3DXVECTOR2(1, 1);
	this->m_iFrameNumber = 1;
	this->m_pTextureSize = D3DXVECTOR2(512, 512);
	this->m_pImageSize = D3DXVECTOR2(512, 512);

	if(this->m_iObjectType == EnvironmentObjectType::Asteroid)
	{
		this->m_iFrameNumber = 64;
		this->m_pImageSize = D3DXVECTOR2(64, 64);
	}
	//initialize the animation if its an asteroid
	if(this->m_iObjectType != EnvironmentObjectType::Planet)
		this->InitializeAnimation();

	//adjust the bounding box 
	D3DXVECTOR2 min = this->m_sPosition + this->m_pScale * -0.5;
	D3DXVECTOR2 max = this->m_sPosition + this->m_pScale * 0.5;
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(min.x, 0, min.y), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(max.x, 0, max.y), sizeof(D3DXVECTOR3));

	//this->m_pPlanetColors = new D3DXVECTOR4[5];
	
	if(this->m_iObjectType == EnvironmentObjectType::Planet)
		this->m_pPlanetColor = D3DXVECTOR4((float)(rand() % 255) / 256.0, (float)(rand() % 255) / 256.0, (float)(rand() % 255) / 256.0, 1);
}

//Deconstructor
CEnvironmentObject::~CEnvironmentObject()
{
	//De-incremement the number of object references for the bullet objects
	CEnvironmentObject::g_iReferences--;

	//If there are no more bullet objects initialized, then de-reference the global variables
	if(CEnvironmentObject::g_iReferences == 0)
	{
		delete CEnvironmentObject::g_pMeshes;
		CEnvironmentObject::g_pMeshes = NULL;

		delete CEnvironmentObject::g_pEffect;
		CEnvironmentObject::g_pEffect = NULL;

		delete [] CEnvironmentObject::g_pTextures;
		CEnvironmentObject::g_pTextures = NULL;

		delete [] CEnvironmentObject::g_pNebulaTextures;
		CEnvironmentObject::g_pNebulaTextures = NULL;

		delete [] CEnvironmentObject::g_pPlanetTextures;
		CEnvironmentObject::g_pPlanetTextures = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
	if(this->m_pTranslation != NULL)
		delete this->m_pTranslation;
	if(this->m_pRotation != NULL)
		delete this->m_pRotation;
}
#pragma endregion

#pragma region Methods

//Load an array of meshes that are directly related to the object's type
CMesh** CEnvironmentObject::LoadMeshes(CGraphicsDevice* device, int size)
{
	CMesh** meshes = new CMesh*[size];

	meshes[EnvironmentObjectType::Asteroid] =		CEnvironmentObject::BuildMesh(device, 3.0f, 3.0f);
	meshes[EnvironmentObjectType::AsteroidNormal] = CEnvironmentObject::BuildMesh(device, 3.0f, 3.0f);
	meshes[EnvironmentObjectType::Planet] =			CEnvironmentObject::BuildMesh(device, 1.0f, 1.0f);
	meshes[EnvironmentObjectType::SpaceStation] =	CEnvironmentObject::BuildMesh(device, 3.0f, 3.0f);

	return meshes;
}

//Load an array of textures that are directly related to the object's type
CTexture2D** CEnvironmentObject::LoadTextures(CGraphicsDevice* device, int size)
{
	// This function should really change such that size and filepath come from the assets portion of the XML. :-D
	CTexture2D** textures = new CTexture2D*[size - 1];

	textures[EnvironmentObjectType::Asteroid] = CTexture2D::Load(device, "..\\Assets\\Art\\asteroids_texture.png");
	textures[EnvironmentObjectType::AsteroidNormal] = CTexture2D::Load(device, "..\\Assets\\Art\\asteroids_normal.png");
	textures[EnvironmentObjectType::SpaceStation] = CTexture2D::Load(device, "..\\Assets\\Art\\spaceStation.png");
	//textures[EnvironmentObjectType::Planet] = CTexture2D::Load(device, "..\\Assets\\Art\\planet_0.png");

	return textures;
}

//Load the array of nebula textures
CTexture2D** CEnvironmentObject::LoadNebulas(CGraphicsDevice* device, int size)
{
	CTexture2D** textures = new CTexture2D*[size];

	char path[256];

	for(int i = 0;  i < size; i++)
	{
		sprintf(&(path[0]), "..\\Assets\\Art\\nebula_%i.png", i);
		textures[i] = CTexture2D::Load(device, &(path[0]));
	}

	return textures;
}

//load the texture layers that make up the planets
CTexture2D** CEnvironmentObject::LoadPlanets(CGraphicsDevice* device, int size)
{
	CTexture2D** textures = new CTexture2D*[size];

	char path[256];
	for(int i = 0; i < size; i++)
	{
		sprintf(&(path[0]), "..\\Assets\\Art\\planet_%i.png", i);
		textures[i] = CTexture2D::Load(device, &(path[0]));
	}

	return textures;
}

// build out the mesh to display the texture for the EnvironmentObject
CMesh* CEnvironmentObject::BuildMesh(CGraphicsDevice* device, float scaleX, float scaleZ)
{
	CMesh* mesh = new CMesh(device);

	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -scaleX;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scaleZ;
	vertices[0].TextureCoord.x = 1;
	vertices[0].TextureCoord.y = 1;

	vertices[1].Position.x = scaleX;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scaleZ;
	vertices[1].TextureCoord.x = 0;
	vertices[1].TextureCoord.y = 1;

	vertices[2].Position.x = -scaleX;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scaleZ;
	vertices[2].TextureCoord.x = 1;
	vertices[2].TextureCoord.y = 0;

	vertices[3].Position.x = scaleX;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scaleZ;
	vertices[3].TextureCoord.x = 0;
	vertices[3].TextureCoord.y = 0;

	mesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	mesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	mesh->SetAttributes((UINT*)attributes, 2);
	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	return mesh;
}

//Create my m_pAnimatedAsteroid
void CEnvironmentObject::InitializeAnimation()
{
	this->m_pAnimation = new CAnimation(this->Get_Game());
	this->m_pAnimation->Set_ImageScale(D3DXVECTOR2(this->m_pScale.x, this->m_pScale.y));
	this->m_pAnimation->Set_ImageSize(this->m_pImageSize);
	this->m_pAnimation->Set_TextureSize(this->m_pTextureSize);
	this->m_pAnimation->Set_Position(this->m_sPosition);
	//this->m_pAnimation->Set_Texture(CEnvironmentObject::g_pTextures[this->m_iObjectType]);
	this->m_pAnimation->Set_FramePerSecond(8);
	this->m_pAnimation->Set_NumberOfFrames(this->m_iFrameNumber);
	this->m_pAnimation->Set_InitialFrame(0);
	this->m_pAnimation->Set_IsLooping(true);
	this->m_pAnimation->Set_WaitOnFirstFrame(false);
	this->Get_Game()->Get_Components()->Add(this->m_pAnimation);
}
#pragma endregion

#pragma region Overriden Methods
// check for collisions; bullets don't collide with objects as a recipient.
bool CEnvironmentObject::CollisionOccured(ICollidableUnit* unit)
{
	return false;
}


//initialize the object
void CEnvironmentObject::Initialize()
{
	CGameUnit::Initialize();

	if(this->m_pAnimation != NULL)
		this->m_pAnimation->Initialize();
}
// load the core components of the object
void CEnvironmentObject::LoadContent()
{
	CGameUnit::LoadContent();

	// initial gathering of all the services
	if(CEnvironmentObject::g_pCamera == NULL)
		CEnvironmentObject::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CEnvironmentObject::g_pCollision == NULL)
		CEnvironmentObject::g_pCollision = (ICollisionService*)this->Get_Game()->Get_Services()->GetService("ICollisionService");
	if(CEnvironmentObject::g_pWorldScroller == NULL)
		CEnvironmentObject::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");
	if(CEnvironmentObject::g_pSceneManager == NULL)
		CEnvironmentObject::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");

	// loading of the effects, textures, and mesh
	if(CEnvironmentObject::g_pEffect == NULL)
		CEnvironmentObject::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\PlanetEffect.fx");

	// Now building meshes like the textures.
	if(CEnvironmentObject::g_pMeshes == NULL)
		CEnvironmentObject::g_pMeshes = CEnvironmentObject::LoadMeshes(this->Get_Game()->Get_GraphicsDevice(), EnvironmentObjectType::Size);

	if(CEnvironmentObject::g_pTextures == NULL)
		CEnvironmentObject::g_pTextures = CEnvironmentObject::LoadTextures(this->Get_Game()->Get_GraphicsDevice(), 3);

	if(CEnvironmentObject::g_pNebulaTextures == NULL)
		CEnvironmentObject::g_pNebulaTextures = CEnvironmentObject::LoadNebulas(this->Get_Game()->Get_GraphicsDevice(), 8);
	
	if(CEnvironmentObject::g_pPlanetTextures == NULL)
		CEnvironmentObject::g_pPlanetTextures = CEnvironmentObject::LoadPlanets(this->Get_Game()->Get_GraphicsDevice(), 5);
	
	if(this->m_pAnimation != NULL)
	{	
		this->m_pAnimation->LoadContent();
		if(this->m_iObjectType == EnvironmentObjectType::Nebula)
			this->m_pAnimation->Set_Texture(CEnvironmentObject::g_pNebulaTextures[rand() % 8]);
		else if(this->m_iObjectType != EnvironmentObjectType::Planet)
			this->m_pAnimation->Set_Texture(CEnvironmentObject::g_pTextures[this->m_iObjectType]);
	}

	if(CEnvironmentObject::g_pWorldScroller == NULL)
		exit(-1);
}

// update the position and drawable position
void CEnvironmentObject::Update(CGameTime* gameTime)
{
	CGameUnit::Update(gameTime);

	//get the world offset
	D3DXVECTOR2 screenCoord = CEnvironmentObject::g_pWorldScroller->ToScreenCoord(this->m_sPosition);
	
	if(this->m_iObjectType != EnvironmentObjectType::Planet)
		this->m_pAnimation->Set_Position(screenCoord);
	
	D3DXMatrixIdentity(this->m_pScaling);
	D3DXMatrixTranslation(this->m_pTranslation, screenCoord.x, 0, screenCoord.y);
	D3DXMatrixScaling(this->m_pScaling, this->m_pScale.x, 0, this->m_pScale.y);

	// update the draw position/rotation of the object
	D3DXMatrixIdentity(this->m_pWorld);

	//if the object is a planet, apply a scalar matrix
	if(this->m_iObjectType == EnvironmentObjectType::Planet)
		D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pScaling);

	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	
	

	// if a collision system exists, then check and see if the bullet has collided, if so kill thyself
	if(CEnvironmentObject::g_pCollision != NULL)
		CEnvironmentObject::g_pCollision->HasCollided(this);
	
	//update the turret
	if(this->m_pAnimation != NULL)
	{
		this->m_pAnimation->Update(gameTime);
		this->m_pAnimation->Set_WorldMatrix(this->m_pWorld);
	}
}

// draw the bullet out to the screen
void CEnvironmentObject::Draw(CGameTime* gameTime)
{
	CGameUnit::Draw(gameTime);

	if(this->m_pAnimation != NULL && this->m_iObjectType != EnvironmentObjectType::Planet)
	{
		this->m_pAnimation->Draw(gameTime);
	}
	else
	{
		// set vector declaration
		this->Get_Game()->Get_GraphicsDevice()->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

		// set the basic effect parameters
		CEnvironmentObject::g_pEffect->Set_View(&CEnvironmentObject::g_pCamera->Get_View());
		CEnvironmentObject::g_pEffect->Set_World(this->m_pWorld);
		CEnvironmentObject::g_pEffect->Set_Projection(&CEnvironmentObject::g_pCamera->Get_Projection());

		// set the texture if one exists
		if(CEnvironmentObject::g_pPlanetTextures != NULL)
		{
			//load the textures
			CEnvironmentObject::g_pEffect->SetVariable("BaseTexture", CEnvironmentObject::g_pPlanetTextures[0]);
			CEnvironmentObject::g_pEffect->SetVariable("GroundTexture", CEnvironmentObject::g_pPlanetTextures[1]);
			CEnvironmentObject::g_pEffect->SetVariable("WaterTexture", CEnvironmentObject::g_pPlanetTextures[2]);
			CEnvironmentObject::g_pEffect->SetVariable("AtmoTexture", CEnvironmentObject::g_pPlanetTextures[3]);
			CEnvironmentObject::g_pEffect->SetVariable("CloundTexture", CEnvironmentObject::g_pPlanetTextures[4]);

			//load the colors
			CEnvironmentObject::g_pEffect->SetVariable("BaseColor", this->m_pPlanetColor);

		}
		CEnvironmentObject::g_pEffect->Begin();
		{
			ID3D10EffectTechnique* technique = CEnvironmentObject::g_pEffect->Get_CurrentTechnique();

			D3D10_TECHNIQUE_DESC td;
			technique->GetDesc(&td);
			for( UINT p = 0; p < td.Passes; ++p )
			{
				// draw the mesh
				technique->GetPassByIndex(p)->Apply(0);
				CEnvironmentObject::g_pMeshes[this->m_iObjectType]->Draw();
			}
		}
		CEnvironmentObject::g_pEffect->End();
	}
}
#pragma endregion