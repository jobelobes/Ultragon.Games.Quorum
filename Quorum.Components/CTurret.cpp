/**
 * @description 
 *   Represents the turrets that display.  This object can fire bullets and rotate
 *   around its axis point.  Also manages what texture to display on the turret.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CTurret::g_iReferences = 0;
int CTurret::g_iTextureCount = 6;

CMesh* CTurret::g_pMesh = NULL;
CEffect* CTurret::g_pEffect = NULL;
CTexture2D** CTurret::g_pTextures = NULL;

ICameraService* CTurret::g_pCamera = NULL;
CQuorumSceneManager* CTurret::g_pSceneManager = NULL;
IWorldScroller* CTurret::g_pWorldScroller = NULL;

const float CTurret::InputRotateRate = D3DX_PI / 1.05;
#pragma endregion

#pragma region Properties
//Return the type of turret
int CTurret::Get_Type()
{
	return this->m_sAttributes.Type;
}

//Set the type of turret
void CTurret::Set_Type(int value)
{
	this->m_sAttributes.Type = value;
}

//Return the cooldown of the turret
int CTurret::Get_Cooldown()
{
	return this->m_sAttributes.Cooldown;
}

//Set the cooldown of the party
void CTurret::Set_Cooldown(int value)
{
	this->m_sAttributes.Cooldown = value;
}

//Return the world transform of the turret
D3DXMATRIX* CTurret::Get_Transform()
{
	return this->m_pWorld;
}

//Sets the world transform of the turret
void CTurret::Set_Transform(D3DXMATRIX transform)
{
	memcpy(this->m_pWorld, &transform, sizeof(D3DXMATRIX));
}
//Return the rotation rate of the turret
float CTurret::Get_RotateRate()
{
	return CTurret::InputRotateRate;
}

//Returns the fire target matrix
D3DXMATRIX* CTurret::Get_FireTarget()
{
	return this->m_pFireTarget;
}

//Returns the attached station
CStation* CTurret::Get_Station()
{
	return this->m_pShipSegment;
}

//Return the shield
CShield* CTurret::Get_Shield()
{
	return this->m_pShield;
}

float CTurret::Get_BarrelRotation()
{
	return this->m_pBarrelRotation;
}

void CTurret::Set_BarrelRotation(float value)
{
	this->m_pBarrelRotation = value;
}
#pragma endregion

#pragma region Overriden Properties
void CTurret::Set_Enabled(bool value)
{
	CGameComponent::Set_Enabled(value);
}

void CTurret::Set_Visible(bool value)
{
	CDrawableGameComponent::Set_Visible(value);

	if(this->m_pAnimatedTurret != NULL)
		this->m_pAnimatedTurret->Set_Visible(value);
}
#pragma endregion

#pragma region IControllableUnit Properties
//Return whether or not the station is bound to a controller
bool CTurret::Get_IsBound()
{
	return this->m_bBound;
}

//Set the turret's bound flag
void CTurret::Set_IsBound(bool value)
{
	this->m_bBound = value;
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Parameterized constructor that passes in a ship segment associated with it, and a custom translation and rotation transformation matrix
CTurret::CTurret(CGame* game, CStation* shipsegment, D3DXMATRIX translation, D3DXMATRIX rotation, TurretAttributes attributes)
: CDrawableGameComponent(game)
{
	//Increment the object references
	CTurret::g_iReferences++;

	//Declare the variables
	this->m_pShipSegment = shipsegment;
	this->m_pTranslation = new D3DXMATRIX(*translation.m);
	this->m_pRotation = new D3DXMATRIX(*rotation.m);
	this->m_pWorld = new D3DXMATRIX();
	this->m_pBarrelRotation = 0.0f;
	this->m_fFireTimer = 0.0f;
	this->m_bBound = false;
	this->m_pFireTarget = new D3DXMATRIX();

	this->m_bIsOkToFire = false;

	this->m_pTarget = NULL;
	this->m_pShield = NULL;
	this->m_pAnimatedTurret = NULL;

	memcpy(&this->m_sAttributes, &attributes, sizeof(TurretAttributes));
}

//Deconstructor
CTurret::~CTurret()
{
	//De-increment the object references
	CTurret::g_iReferences--;

	//if the object references is 0, de-reference the global variables
	if(CTurret::g_iReferences == 0)
	{
		delete CTurret::g_pMesh;
		CTurret::g_pMesh = NULL;

		delete CTurret::g_pEffect;
		CTurret::g_pEffect = NULL;

		delete [] CTurret::g_pTextures;
		CTurret::g_pTextures = NULL;
	}

	//reset the world transforms
	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
	if(this->m_pTranslation != NULL)
		delete this->m_pTranslation;
	if(this->m_pRotation != NULL)
		delete this->m_pRotation;
}
#pragma endregion

#pragma region Methods

//Create my m_pTurretAnimation
void CTurret::InitializeTurretAnimation()
{
	this->m_pAnimatedTurret = new CAnimation(this->Get_Game());
	this->m_pAnimatedTurret->Set_ImageScale(D3DXVECTOR2(2,1));
	this->m_pAnimatedTurret->Set_ImageSize(D3DXVECTOR2(64,32));
	this->m_pAnimatedTurret->Set_TextureSize(D3DXVECTOR2(512,32));
	this->m_pAnimatedTurret->Set_Position(D3DXVECTOR2(0, 0));
//	this->m_pAnimatedTurret->Set_Texture(CTurret::g_pTextures[this->m_sAttributes.Type]);
	this->m_pAnimatedTurret->Set_FramePerSecond(int(8/this->m_sAttributes.Cooldown));
	this->m_pAnimatedTurret->Set_NumberOfFrames(8);
	this->m_pAnimatedTurret->Set_InitialFrame(0);
	this->m_pAnimatedTurret->Set_IsLooping(false);
	this->m_pAnimatedTurret->Set_WaitOnFirstFrame(true);
	this->Get_Game()->Get_Components()->Add(this->m_pAnimatedTurret);
}

//Create a bullet object and fire it into the world
void CTurret::Fire()
{
	//If the cooldown is up, its ok to fire
	if(this->m_bIsOkToFire)
	{
		//Create new translation matrices that are offsets of the turret's world transforms
		D3DXVECTOR2 position = D3DXVECTOR2(this->m_pWorld->_41, this->m_pWorld->_43);

		D3DXVECTOR2 velocity = D3DXVECTOR2(this->m_pFireTarget->_41, this->m_pFireTarget->_43);
		D3DXVec2Subtract(&velocity, &velocity, &position);
		D3DXVec2Normalize(&velocity, &velocity);
		velocity = velocity * this->m_sAttributes.Speed;
		position = CTurret::g_pWorldScroller->ToWorldCoord(position);

		//Create a new bullet object
		CTurret::g_pSceneManager->CreateBullet(position, velocity, BulletOwner::Owner_Ship, (BulletType)this->m_sAttributes.Type, this->m_sAttributes.Lifespan);

		//Reset the cooldown flag
		this->m_bIsOkToFire = false;

		this->m_pAnimatedTurret->Start(true);

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX introLoop;
		
		

		// play the sound dependent upon turret type
		int theType = this->Get_Type();
			
		if(theType == 0)
			s->GetCueIndex("turret_0", &introLoop);
		else if(theType == 1)
			s->GetCueIndex("turret_1", &introLoop);
		else if(theType == 2)
			s->GetCueIndex("turret_2", &introLoop);
		else
			s->GetCueIndex("turret_3", &introLoop);

		s->PlaySound(introLoop);
	}
}

//Rotate the turret based on an angle offset
void CTurret::Rotate(float value)
{
	this->m_pBarrelRotation += value;
	this->m_pBarrelRotation = min(D3DX_PI/4, this->m_pBarrelRotation);
	this->m_pBarrelRotation = max(-D3DX_PI/4, this->m_pBarrelRotation);
}

//Rotate the turret based on an angle offset
void CTurret::RotateAbs(float value)
{
	this->m_pBarrelRotation = value;
	this->m_pBarrelRotation = min(D3DX_PI/4, this->m_pBarrelRotation);
	this->m_pBarrelRotation = max(-D3DX_PI/4, this->m_pBarrelRotation);
}

//Rotate the shield
void CTurret::RotateShield(float value)
{
	this->m_pShield->Rotate(value);
}

void CTurret::Set_Color(int color)
{
	this->m_pShield->Set_Color(color);
}

//Binds the turret to a target object
void CTurret::BindTarget(CTarget* target)
{
	this->m_pTarget = target;
}

//Unbinds the turret from its current target object
void CTurret::UnbindTarget()
{
	if(this->m_pTarget != NULL)
		this->m_pTarget = NULL;
}
#pragma endregion

#pragma region Static Methods
//Load an array of textures that are directly related to the turret's type
CTexture2D** CTurret::LoadTextures(CGraphicsDevice* device, int size)
{
	char path[256];
	CTexture2D** textures = new CTexture2D*[size];
	for(int i = 0; i < size; i++)
	{
		sprintf(&(path[0]), "..\\Assets\\Art\\turret_ani_%i.png", i);
		textures[i] = CTexture2D::Load(device, &(path[0]));
	}
	return textures;
}

//Build the static mesh
CMesh* CTurret::BuildMesh(CGraphicsDevice* device, float scaleX, float scaleZ)
{
	CMesh* mesh = new CMesh(device);
	float scale = 2.0f;

	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.5f * scaleX;
	vertices[0].Position.y = 0 * scale;
	vertices[0].Position.z = -0.25f * scaleZ;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 0.5f * scaleX;
	vertices[1].Position.y = 0 * scale;
	vertices[1].Position.z = -0.25f * scaleZ;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -0.5f * scaleX;
	vertices[2].Position.y = 0 * scale;
	vertices[2].Position.z = 0.25f * scaleZ;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 0.5f * scaleX;
	vertices[3].Position.y = 0 * scale;
	vertices[3].Position.z = 0.25f * scaleZ;
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

//return the lifespan of the bullet based on a type
float CTurret::Get_StaticLifespan(int type)
{
	std::vector<TURRET*> turrets;

	CGameContent* gameContent = CTurret::g_pSceneManager->Get_GameContent();
	gameContent->Get_Turrets(turrets);

	for(int i = 0; i < turrets.size(); i++)
	{	
		if(turrets[i]->type == type)
			return turrets[i]->lifespan;
	}

	return NULL;
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the turret object
void CTurret::Initialize()
{
	CDrawableGameComponent::Initialize();

	if(this->m_pShipSegment->Get_StationType() == StationType::Shield)
	{
		this->m_pShield = new CShield(this->Get_Game(), CQuorumIdentifiers::Quorum_Shield);
		this->m_pShield->Set_Owner(this->m_pShipSegment->Get_Ship());
		this->Get_Game()->Get_Components()->Add(this->m_pShield);
	}
	else
	{
		this->m_pShield = NULL;
	}
	if(this->m_pShield != NULL)
		this->m_pShield->Initialize();

	this->InitializeTurretAnimation();

	if(this->m_pAnimatedTurret != NULL)
		this->m_pAnimatedTurret->Initialize();

}

//Load content associated to the turret objects
void CTurret::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	if(CTurret::g_pCamera == NULL)
		CTurret::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CTurret::g_pSceneManager == NULL)
		CTurret::g_pSceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	if(CTurret::g_pWorldScroller == NULL)
		CTurret::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");


	if(CTurret::g_pEffect == NULL)
		CTurret::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\DefaultTextureEffect.fx");
	if(CTurret::g_pTextures == NULL)
		CTurret::g_pTextures = CTurret::LoadTextures(device, CTurret::g_iTextureCount);
	if(CTurret::g_pMesh == NULL)
		CTurret::g_pMesh = CTurret::BuildMesh(device, 2.0f, 2.0f);

	if(this->m_pShield != NULL)
		this->m_pShield->LoadContent();

	

	if(this->m_pAnimatedTurret != NULL)
	{	
		this->m_pAnimatedTurret->LoadContent();
		this->m_pAnimatedTurret->Set_Texture(CTurret::g_pTextures[this->m_sAttributes.Type]);
	}
}

//Update the turret's world transform
void CTurret::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	//if the station is a firing turret, update with the target's position
	if(this->m_pShipSegment->Get_StationType() == StationType::Turret)
	{
		//adjust the turret's rotation based on the position of the target
		if(this->m_pTarget != NULL)
		{
			//Get the position points of target and current so we can subtract them to get the angle offset
			D3DXVECTOR3 pTarget = D3DXVECTOR3(this->m_pTarget->Get_Transform()->_41, this->m_pTarget->Get_Transform()->_42, this->m_pTarget->Get_Transform()->_43);
			
			//Get the world tranform of the turret
			D3DXMATRIX* turretMatrix = this->m_pWorld;

			//Get the turret's fire point matrix
			D3DXMATRIX* shipMatrix = this->m_pShipSegment->Get_Ship()->Get_Transform();

			//Get the target position of the turret
			D3DXVECTOR3 pTurretEye = D3DXVECTOR3(turretMatrix->_41, turretMatrix->_42, turretMatrix->_43);
			D3DXVECTOR3 pShipOrigin = D3DXVECTOR3(shipMatrix->_41, shipMatrix->_42, shipMatrix->_43);
			
			//Get the direction vector between the turret's eye and the fire target position
			D3DXVECTOR3 turretDir;
			D3DXVec3Subtract(&turretDir, &pTurretEye, &pShipOrigin);
			D3DXVec3Normalize(&turretDir, &turretDir);

			//Get the direction vector between the turret's eye and the target reticule's position
			D3DXVECTOR3 targetDir;
			D3DXVec3Subtract(&targetDir, &pTarget, &pTurretEye);
			D3DXVec3Normalize(&targetDir, &targetDir);

			//Grab the angle offset from the dot product of the two
			float angle = D3DXVec3Dot(&targetDir, &turretDir);
			
			//checks to see if the target is behind the target
			if(angle > 0)
			{
				//update the barrel rotation
				if(_finite(acos(angle)) != 0)
					this->m_pBarrelRotation  = acos(angle);
				else
					this->m_pBarrelRotation  = acos(angle);
				
				D3DXVECTOR3 cross;
				D3DXVec3Cross(&cross, &turretDir, &targetDir);

				//if the target is below the base axis plane, multiply the angle by -1
				if(cross.y < 0)
					this->m_pBarrelRotation *= -1;

				this->m_pBarrelRotation = min(D3DX_PI/4, this->m_pBarrelRotation);
				this->m_pBarrelRotation = max(-D3DX_PI/4, this->m_pBarrelRotation);
			}
		}

		if(this->m_pShield != NULL)
		{
			this->m_pShield->Set_Enabled(false);
			this->m_pShield->Set_Bound(false);
		}

	//if the station type is a shield station, update the shield's world matrix 
	}else if(this->m_pShipSegment->Get_StationType() == StationType::Shield && this->m_bBound)
	{
		if(this->m_pShield != NULL)
		{
			this->m_pShield->Set_Enabled(true);
			this->m_pShield->Update(gameTime);
		}
	}

	// update the segment's position
	D3DXMatrixIdentity(this->m_pWorld);
	
	// local rotation
	D3DXMATRIX tmp;
	D3DXMATRIX tmp2;

	D3DXMatrixTranslation(&tmp, 0.5f, 0, 0);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, &tmp);

	D3DXMatrixRotationAxis(&tmp, &D3DXVECTOR3(0,1,0), this->m_pBarrelRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, &tmp);

	// world matrix
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);

	// segment offset matrix
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pShipSegment->Get_Transform());

	//Tracks a point in front of the turrets. Used for firing bullets
	D3DXMatrixIdentity(this->m_pFireTarget);	
	D3DXMatrixTranslation(&tmp2, 0.3f, 0, 0);
	D3DXMatrixMultiply(this->m_pFireTarget, this->m_pFireTarget, &tmp2);
	D3DXMatrixMultiply(this->m_pFireTarget, this->m_pFireTarget, this->m_pWorld);

	//update the turret animation's position
	if(this->m_pAnimatedTurret != NULL)
		this->m_pAnimatedTurret->Set_WorldMatrix(this->m_pWorld);

	// update the turret's cooldown
	if(this->m_fFireTimer >= this->m_sAttributes.Cooldown)
	{
		this->m_bIsOkToFire = true;
		this->m_pAnimatedTurret->Start(true);
		this->m_pAnimatedTurret->Pause(true);
		this->m_fFireTimer = 0.0f;
	}else{
		//this->m_pAnimatedTurret->Start(false);
		this->m_fFireTimer += gameTime->Get_ElapsedTime();
	}

	if(this->m_pAnimatedTurret != NULL)
		this->m_pAnimatedTurret->Update(gameTime);
}

//Draw the turret based on the updated transform
void CTurret::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	//if the station is the shield station, then draw the shield
	if(this->m_pShipSegment->Get_StationType() == StationType::Shield && this->m_bBound)
	{
		this->m_pShield->Set_Visible(true);
		this->m_pShield->Draw(gameTime);
	}
	else
	{
		if(this->m_pShield != NULL)
			this->Set_Visible(true);
	}

	if(this->m_pAnimatedTurret != NULL)
		this->m_pAnimatedTurret->Draw(gameTime);

}
#pragma endregion