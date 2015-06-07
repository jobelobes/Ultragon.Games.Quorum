/**
 * @description 
 *   This is the enemy unit of the game.  The class can be bound to via an AI
 *   controller for dynamic movement and behaviors.  The quintisential enemy of the game.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CEnemyUnit::g_iReferences = 0;
CMesh* CEnemyUnit::g_pMesh = NULL;
CEffect* CEnemyUnit::g_pEffect = NULL;
CTexture2D* CEnemyUnit::g_pTexture = NULL;

ICameraService* CEnemyUnit::g_pCamera = NULL;
IWorldScroller* CEnemyUnit::g_pWorldScroller = NULL;
ICollisionService* CEnemyUnit::g_pCollision = NULL;
CQuorumSceneManager* CEnemyUnit::g_pSceneManager = NULL;

const float CEnemyUnit::InputMoveRate = 15;
const float CEnemyUnit::InputRotateRate = 10;
#pragma endregion

#pragma region Properties
//Returns the health of the enemy unit
int CEnemyUnit::Get_Health()
{
	return this->m_sAttributes.Health;
}

//Set a value for the health
void CEnemyUnit::Set_Health(int value)
{
	this->m_sAttributes.Health = value;
}

//Return the damage that the enemy unit is capable of causing.
int CEnemyUnit::Get_Damage()
{
	return this->m_sAttributes.Damage;
}

//Return the fire radius of the enemy unit
int CEnemyUnit::Get_FireRadius()
{
	return this->m_sAttributes.FireRadius;
}

//Return the type of enemy unit
int CEnemyUnit::Get_Type()
{
	return this->m_sAttributes.Type;
}

CShip* CEnemyUnit::Get_Target()
{
	return this->m_pTarget;
}

void CEnemyUnit::Set_Target(CShip* ship)
{
	this->m_pTarget = ship;
}

bool CEnemyUnit::Get_IsOkToFire()
{
	return this->m_bIsOkToFire;
}

void CEnemyUnit::Set_IsOkToFire(bool isFire)
{
	this->m_bIsOkToFire = isFire;
}

float CEnemyUnit::Get_RotationAngle()
{
	return m_fRotation;
}
#pragma endregion

#pragma region IBasicUnit Properties
//Get the unit's movement rate
float CEnemyUnit::Get_MoveRate()
{
	return CEnemyUnit::InputMoveRate;
}

//Get the unit's rotation rate
float CEnemyUnit::Get_RotateRate()
{
	return CEnemyUnit::InputRotateRate;
}

//Get the unit's position
D3DXVECTOR2 CEnemyUnit::Get_Position()
{
	return this->m_sPosition;
}

//Get the unit's position
void CEnemyUnit::Set_Position(D3DXVECTOR2 value)
{
	memcpy(&this->m_sPosition, &value, sizeof(D3DXVECTOR2));

	D3DXVECTOR2 screenCoord = CEnemyUnit::g_pWorldScroller->ToScreenCoord(this->m_sPosition);
	D3DXMatrixTranslation(this->m_pTranslation, screenCoord.x, 0, screenCoord.y);

	//Adjust the position based on a world scroller offset
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 1.0f, 0, this->m_sPosition.y - 1.0f), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 1.0f, 0, this->m_sPosition.y + 1.0f), sizeof(D3DXVECTOR3));
}

//Set the unit's position
float CEnemyUnit::Get_Rotation()
{
	return this->m_fRotation;
}

//Set the unit's position
void CEnemyUnit::Set_Rotation(float value)
{
	this->Rotate(value);
}
#pragma endregion

#pragma region IControllableUnit Properties
//Return whether or not the unit is bound to a controller
bool CEnemyUnit::Get_IsBound()
{
	return this->m_bBound;
}

//Set the unit's bound flag
void CEnemyUnit::Set_IsBound(bool value)
{
	this->m_bBound = value;
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CEnemyUnit::CEnemyUnit(CGame* game, int identifier)
: CGameUnit(game, identifier)
{
	//Incrememnt the object's references
	CEnemyUnit::g_iReferences++;

	//Declare the variables
	this->m_bBound = false;
	this->m_bIsHit = false;
	this->m_fHitCountdown = 0.0f;

	this->m_fRotation = 0.0f;
	ZeroMemory(&this->m_sPosition, sizeof(D3DXVECTOR2));

	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(-1.0f,0,-1.0f), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(1.0f,0,1.0f), sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());

	this->m_sAttributes.Health = 100;
	this->m_sAttributes.Damage = 5;
	this->m_sAttributes.FireRadius = 3;
	this->m_sAttributes.Type = 1;
	this->m_sAttributes.Cooldown = 1.0;

	this->m_pTarget = NULL;

	this->m_fFireTimer = 0.0f;
	this->m_bIsOkToFire = false;
}

//Deconstructor
CEnemyUnit::~CEnemyUnit() 
{
	//De-increment the references
	CEnemyUnit::g_iReferences--;

	//if there are no more enemy unit references, de-reference the global variables
	if(CEnemyUnit::g_iReferences == 0)
	{
		delete CEnemyUnit::g_pMesh;
		CEnemyUnit::g_pMesh = NULL;

		delete CEnemyUnit::g_pEffect;
		CEnemyUnit::g_pEffect = NULL;

		delete CEnemyUnit::g_pTexture;
		CEnemyUnit::g_pTexture = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;

	if(this->m_pTranslation != NULL)
		delete this->m_pTranslation;

	if(this->m_pRotation != NULL)
		delete this->m_pRotation;
}
#pragma endregion

#pragma region IBasicUnit Methods
//Move the unit based on an offset vector
void CEnemyUnit::Move(D3DXVECTOR2 value)
{
	this->m_sPosition += value;

	D3DXVECTOR2 screenCoord = CEnemyUnit::g_pWorldScroller->ToScreenCoord(this->m_sPosition);
	D3DXMatrixTranslation(this->m_pTranslation, screenCoord.x, 0, screenCoord.y);

	//Adjust the position based on a world scroller offset
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 1.0f, 0, this->m_sPosition.y - 1.0f), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 1.0f, 0, this->m_sPosition.y + 1.0f), sizeof(D3DXVECTOR3));
}

//Rotate the unit based on an angle offset
void CEnemyUnit::Rotate(float value)
{
	//Adjust the rotation matrix by the angle offset
	this->m_fRotation = value;
	
	D3DXMatrixRotationY(this->m_pRotation, this->m_fRotation);
}

//RFires at the given target
void CEnemyUnit::Fire(D3DXVECTOR2 dir)
{
	//Fires at the given target
	//Create a new bullet object
	D3DXVECTOR2 velocity = D3DXVECTOR2(dir.x, dir.y);
	D3DXVec2Normalize(&velocity, &velocity);
	velocity = velocity * 20.0f;

	CBullet* bullet = CEnemyUnit::g_pSceneManager->CreateBullet(this->m_sPosition, velocity, BulletOwner::Owner_Enemy, BulletType::Hellfire,3);
	if(bullet != NULL)
	{
		bullet->Set_Damage(5);
	}
}
#pragma endregion

#pragma region Overriden Methods
//Check to see if the unit collided with the passed in collidable object
bool CEnemyUnit::CollisionOccured(ICollidableUnit* unit)
{
	CBullet* bullet = dynamic_cast<CBullet*>(unit);
	if(bullet == NULL)
	{
		// might be a environ obj
		return false;
	}

	//Check for explosion status
	if(bullet != NULL && bullet->Get_Owner() != BulletOwner::Owner_Enemy)
	{
		this->m_sAttributes.Health -= bullet->Get_Damage();
		
		if(this->m_sAttributes.Health > 0)
			this->m_bIsHit = true;
		else
		{
			CAnimation* explosion = (CAnimation*)CEnemyUnit::g_pSceneManager->Create(GameObject::Explosion);
			explosion->Set_Position(CEnemyUnit::g_pWorldScroller->ToScreenCoord(this->m_sPosition));
			explosion->Start();

			CEnemyUnit::g_pSceneManager->Destroy(GameObject::Enemy, this);
		}

		return true;
	}
	return false;
}

//Initialize the enemy unit
void CEnemyUnit::Initialize()
{
	//Call CGameUnit's initalize
	CGameUnit::Initialize();
}

//Load content associated to enemy units
void CEnemyUnit::LoadContent()
{
	//Call CGameUnit's LoadContent
	CGameUnit::LoadContent();

	//If the global variables are not set, load the content associated to them
	if(CEnemyUnit::g_pCamera == NULL)
		CEnemyUnit::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CEnemyUnit::g_pCollision == NULL)
		CEnemyUnit::g_pCollision = (ICollisionService*)this->Get_Game()->Get_Services()->GetService("ICollisionService");
	if(CEnemyUnit::g_pWorldScroller == NULL)
		CEnemyUnit::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");
	if(CEnemyUnit::g_pSceneManager == NULL)
		CEnemyUnit::g_pSceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	
	if(CEnemyUnit::g_pEffect == NULL)
		CEnemyUnit::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\EnemyEffect.fx");
	if(CEnemyUnit::g_pTexture == NULL)
		CEnemyUnit::g_pTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\enemy_1.png");
	if(CEnemyUnit::g_pMesh == NULL)
		CEnemyUnit::BuildMesh(this->Get_Game()->Get_GraphicsDevice());
}

//Update the unit's position and rotation
void CEnemyUnit::Update(CGameTime* gameTime)
{
	//Call CGameUNit's update
	CGameUnit::Update(gameTime);

	// update the segment's position
	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);

	if(this->m_bIsHit)
	{
		if(this->m_fHitCountdown > 0.3f)
		{
			this->m_bIsHit = false;
			this->m_fHitCountdown = 0.0f;
		}
		else
			this->m_fHitCountdown += 0.1f;
	}

	// update the enemy's cooldown
	if(this->m_fFireTimer >= this->m_sAttributes.Cooldown)
	{
		this->m_bIsOkToFire = true;
		this->m_fFireTimer = 0.0f;
	}
	else
		this->m_fFireTimer += gameTime->Get_ElapsedTime();
}	

//Draw the enemy unit
void CEnemyUnit::Draw(CGameTime* gameTime)
{
	//Call CGameUnit's draw
	CGameUnit::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	//Update the effect's parameters
	CEnemyUnit::g_pEffect->Set_View(&CEnemyUnit::g_pCamera->Get_View());
	CEnemyUnit::g_pEffect->Set_World(this->m_pWorld);
	CEnemyUnit::g_pEffect->Set_Projection(&CEnemyUnit::g_pCamera->Get_Projection());
	if(CEnemyUnit::g_pTexture != NULL)
		CEnemyUnit::g_pEffect->SetVariable("InputTexture", CEnemyUnit::g_pTexture);
	CEnemyUnit::g_pEffect->SetVariable("IsHit", this->m_bIsHit);

	//Run through the effect's draw sequence
	CEnemyUnit::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CEnemyUnit::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CEnemyUnit::g_pMesh->Draw();
		}
	}
	CEnemyUnit::g_pEffect->End();
}
#pragma endregion

#pragma region Static Methods
//Build he mesh associated to the enemy unit
void CEnemyUnit::BuildMesh(CGraphicsDevice* device)
{
	if(CEnemyUnit::g_pMesh != NULL)
		delete CEnemyUnit::g_pMesh;

	CEnemyUnit::g_pMesh = new CMesh(device);

	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -1.0f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -1.0f;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 1.0f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -1.0f;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -1.0f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = 1.0f;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 1.0f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 1.0f;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	CEnemyUnit::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CEnemyUnit::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CEnemyUnit::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CEnemyUnit::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion
