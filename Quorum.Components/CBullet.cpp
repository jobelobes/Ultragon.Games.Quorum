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
int CBullet::g_iReferences = 0;
int CBullet::g_iTextureCount = 4;

CMesh* CBullet::g_pMesh = NULL;
CEffect* CBullet::g_pEffect = NULL;
CTexture2D** CBullet::g_pTextures = NULL;

ICameraService* CBullet::g_pCamera = NULL;
ICollisionService* CBullet::g_pCollision = NULL;
IWorldScroller* CBullet::g_pWorldScroller = NULL;
CQuorumSceneManager* CBullet::g_pSceneManager= NULL;
#pragma endregion

#pragma region Properties
// return the bullets initial launcher
BulletOwner CBullet::Get_Owner()
{
	return this->m_eOwner;
}

// sets who initially fired the bullet
void CBullet::Set_Owner(BulletOwner value)
{
	this->m_eOwner = value;
	this->m_fAge = 0;
}

// return the bullet's position
D3DXVECTOR2 CBullet::Get_Position()
{
	return this->m_sPosition;
}

// return the bullet's position
void CBullet::Set_Position(D3DXVECTOR2 value)
{
	memcpy(&this->m_sPosition, &value, sizeof(D3DXVECTOR2));
}

// return the bullet's velocity
D3DXVECTOR2 CBullet::Get_Velocity()
{
	return this->m_sVelocity;
}

// set the bullet's velocity
void CBullet::Set_Velocity(D3DXVECTOR2 value)
{
	memcpy(&this->m_sVelocity, &value, sizeof(D3DXVECTOR2));

	D3DXMatrixRotationY(this->m_pRotation, D3DX_PI - atan2(this->m_sVelocity.y, this->m_sVelocity.x));
}

//Returns the damage
int CBullet::Get_Damage()
{
	return this->m_iImpactDamage;
}

//Set the impact damage
void CBullet::Set_Damage(int value)
{
	this->m_iImpactDamage = value;
}

//Return the animation
CAnimation* CBullet::Get_Animation()
{
	return this->m_pAnimation;
}

//Return the type of bullet
BulletType CBullet::Get_BulletType()
{
	return this->m_iType;
}

//Set the type
void CBullet::Set_BulletType(BulletType value)
{
	if(this->m_iType != value)
	{
		std::vector<TURRET*> turrets;
		CBullet::g_pSceneManager->Get_GameContent()->Get_Turrets(turrets);
		this->m_iImpactDamage = turrets[(int)value]->damage;
		this->m_fLifespan = turrets[(int)value]->lifespan;
		this->m_iType = value;
		this->m_pAnimation->Set_Texture(CBullet::g_pTextures[this->m_iType]);
	}
}

//Return the lifespan of the bullet
float CBullet::Get_Lifespan()
{
	return this->m_fLifespan;
}

//Set the lifespan of the bullet
void CBullet::Set_Lifespan(float value)
{
	this->m_fLifespan = value;
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CBullet::CBullet(CGame* game, int identifier)
	: CGameUnit(game, identifier)
{
	//Declare the variables
	CBullet::g_iReferences++;

	this->m_fAge = 0;
	this->m_iImpactDamage = 0;
	this->m_fLifespan = 1.5f;

	this->Set_Enabled(false);
	this->Set_Visible(false);

	ZeroMemory(&this->m_sPosition, sizeof(D3DXVECTOR2));
	ZeroMemory(&this->m_sVelocity, sizeof(D3DXVECTOR2));
	
	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());

	this->m_iType = BulletType::Hellfire;
	this->m_pAnimation = NULL;

	this->InitializeAnimation();
}

//Deconstructor
CBullet::~CBullet()
{
	//De-incremement the number of object references for the bullet objects
	CBullet::g_iReferences--;

	//If there are no more bullet objects initialized, then de-reference the global variables
	if(CBullet::g_iReferences == 0)
	{
		delete CBullet::g_pMesh;
		CBullet::g_pMesh = NULL;

		delete CBullet::g_pEffect;
		CBullet::g_pEffect = NULL;

		delete [] CBullet::g_pTextures;
		CBullet::g_pTextures = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
}
#pragma endregion

#pragma region Methods
//Initialize the animation cycle of the bullets
void CBullet::InitializeAnimation()
{
	this->m_pAnimation = new CAnimation(this->Get_Game());
	this->m_pAnimation->Set_ImageScale(D3DXVECTOR2(1,1));
	this->m_pAnimation->Set_ImageSize(D3DXVECTOR2(64,32));
	this->m_pAnimation->Set_TextureSize(D3DXVECTOR2(512,32));
	this->m_pAnimation->Set_Position(this->Get_Position());
//	this->m_pAnimation->Set_Texture(CTurret::g_pTextures[this->m_sAttributes.Type]);
	this->m_pAnimation->Set_FramePerSecond(8);
	this->m_pAnimation->Set_NumberOfFrames(8);
	this->m_pAnimation->Set_InitialFrame(0);
	this->m_pAnimation->Set_IsLooping(true);
	this->m_pAnimation->Set_WaitOnFirstFrame(true);
	this->Get_Game()->Get_Components()->Add(this->m_pAnimation);
}

#pragma endregion

#pragma region Overriden Methods
// check for collisions; bullets don't collide with objects as a recipient.
bool CBullet::CollisionOccured(ICollidableUnit*)
{
	return false;
}

// load the core components of the object
void CBullet::LoadContent()
{
	CGameUnit::LoadContent();

	// initial gathering of all the services
	if(CBullet::g_pCamera == NULL)
		CBullet::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CBullet::g_pCollision == NULL)
		CBullet::g_pCollision = (ICollisionService*)this->Get_Game()->Get_Services()->GetService("ICollisionService");
	if(CBullet::g_pWorldScroller == NULL)
		CBullet::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");
	if(CBullet::g_pSceneManager == NULL)
		CBullet::g_pSceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");

	// loading of the effects, textures, and mesh
	if(CBullet::g_pEffect == NULL)
		CBullet::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultTextureEffect.fx");
	if(CBullet::g_pTextures == NULL)
		CBullet::g_pTextures = CBullet::LoadTextures(this->Get_Game()->Get_GraphicsDevice(), CBullet::g_iTextureCount);
	if(CBullet::g_pMesh == NULL)
		CBullet::BuildMesh(this->Get_Game()->Get_GraphicsDevice());

	if(this->m_pAnimation != NULL)
	{
		this->m_pAnimation->LoadContent();
		this->m_pAnimation->Set_Texture(CBullet::g_pTextures[this->m_iType]);

	}
}

// update the position and drawable position
void CBullet::Update(CGameTime* gameTime)
{
	CGameUnit::Update(gameTime);

	// kill off any old bullets
	if((this->m_fAge += gameTime->Get_ElapsedTime()) > this->m_fLifespan)
	{
		CBullet::g_pSceneManager->Destroy(GameObject::Bullet, this);
		return;
	}

	// update position based off velocity
	this->m_sPosition += this->m_sVelocity * gameTime->Get_ElapsedTime();

	D3DXVECTOR2 screenCoord = CBullet::g_pWorldScroller->ToScreenCoord(this->m_sPosition);
	D3DXMatrixTranslation(this->m_pTranslation, screenCoord.x, 0, screenCoord.y);

	// get the world map offset and recalculate the bounding box; needed for collision detection
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 0.25, 0, this->m_sPosition.y - 0.25f), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 0.25, 0, this->m_sPosition.y + 0.25f), sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);

	// if a collision system exists, then check and see if the bullet has collided, if so kill thyself
	if(CBullet::g_pCollision != NULL && CBullet::g_pCollision->HasCollided(this))
	{
		CBullet::g_pSceneManager->Destroy(GameObject::Bullet, this);
		this->m_pAnimation->Stop();
	}

	if(this->m_pAnimation != NULL)
	{
		this->m_pAnimation->Update(gameTime);
		this->m_pAnimation->Set_WorldMatrix(this->m_pWorld);
	}
}

// draw the bullet out to the screen
void CBullet::Draw(CGameTime* gameTime)
{
	CGameUnit::Draw(gameTime);

	if(this->m_pAnimation != NULL)
		this->m_pAnimation->Draw(gameTime);
}
#pragma endregion

#pragma region Static Methods
// load out the textures required for the different types of bullets
CTexture2D** CBullet::LoadTextures(CGraphicsDevice* device, int size)
{
	char path[256];
	CTexture2D** textures = new CTexture2D*[size];
	for(int i = 0; i < size; i++)
	{
		sprintf(&(path[0]), "..\\Assets\\Art\\bullet_ani_%i.png", i);
		textures[i] = CTexture2D::Load(device, &(path[0]));
	}
	return textures;
}

// build out the mesh to display the texture for the bullet
void CBullet::BuildMesh(CGraphicsDevice* device)
{
	if(CBullet::g_pMesh != NULL)
		delete CBullet::g_pMesh;

	CBullet::g_pMesh = new CMesh(device);
	float scaleX = 1.0f;
	float scaleY = 0.5f;

	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -scaleX;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scaleY;
	vertices[0].TextureCoord.x = 1;
	vertices[0].TextureCoord.y = 1;

	vertices[1].Position.x = scaleX;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scaleY;
	vertices[1].TextureCoord.x = 0;
	vertices[1].TextureCoord.y = 1;

	vertices[2].Position.x = -scaleX;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scaleY;
	vertices[2].TextureCoord.x = 1;
	vertices[2].TextureCoord.y = 0;

	vertices[3].Position.x = scaleX;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scaleY;
	vertices[3].TextureCoord.x = 0;
	vertices[3].TextureCoord.y = 0;

	CBullet::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CBullet::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CBullet::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CBullet::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion