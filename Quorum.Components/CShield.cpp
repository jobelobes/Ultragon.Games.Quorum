#include "Quorum.Components.h"

#pragma region Static Variables
int CShield::g_iReferences = 0;

CMesh* CShield::g_pMesh = NULL;
CEffect* CShield::g_pEffect = NULL;
CTexture2D* CShield::g_pTexture;
CTexture2D* CShield::g_pNoiseTexture;
CTexture2D* CShield::g_pShieldMask;

ICameraService* CShield::g_pCamera = NULL;
ICollisionService* CShield::g_pCollision = NULL;
ISceneManager* CShield::g_pSceneManager = NULL;

const float CShield::InputRotateRate = D3DX_PI / 1.05;
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CShield::CShield(CGame* game, int identifier)
	:CGameUnit(game, identifier)
{
	CShield::g_iReferences++;

	this->m_fShieldRotation = 0.0f;
	this->m_pPosition;

	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation =  new D3DXMATRIX());

	this->m_fNoiseX = 0.0f;
	this->m_fNoiseY = 0.0f;

	this->m_iColor = 5;

	//variables to adjust the bounding box as it rotates around the ship
	this->m_fOffX = 1.1f;
	this->m_fOffZ = 3.2f;

	this->m_bBound = false;
	this->Set_Enabled(false);
	this->Set_Visible(false);
	this->m_bIncrementUp = true;

	this->m_pOwner = NULL;
}

//Default Deconstructor
CShield::~CShield()
{
	CShield::g_iReferences--;

	if(CShield::g_iReferences == 0)
	{
		delete CShield::g_pMesh;
		CShield::g_pMesh = NULL;

		delete CShield::g_pTexture;
		CShield::g_pTexture = NULL;

		delete CShield::g_pNoiseTexture;
		CShield::g_pNoiseTexture = NULL;

		delete CShield::g_pEffect;
		CShield::g_pEffect = NULL;

		delete CShield::g_pShieldMask;
		CShield::g_pShieldMask = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
}	
#pragma endregion

#pragma region Properties
//Return the world transform of this shield object
D3DXMATRIX* CShield::Get_Transform()
{
	return this->m_pWorld;
}

//Return the rotate rate of the shield
float CShield::Get_RotateRate()
{
	return CShield::InputRotateRate;
}

//Return the position of the shield
D3DXVECTOR2 CShield::Get_Position()
{
	this->m_pPosition = D3DXVECTOR2(this->m_pWorld->_41, this->m_pWorld->_43);
	
	return this->m_pPosition;
}

//Return the owner of the shield
CGameUnit* CShield::Get_Owner()
{
	return this->m_pOwner;
}

//Set the owner of the shield
void CShield::Set_Owner(CGameUnit* unit)
{
	this->m_pOwner = unit;
}

//Return the shield's bound status
bool CShield::Get_Bound()
{
	return this->m_bBound;
}

//Set the shield's bound flag
void CShield::Set_Bound(bool value)
{
	this->m_bBound = value;
	this->Set_Enabled(value);
	this->Set_Visible(value);
}
#pragma endregion

#pragma region Methods
//Rotate the shield based on an angle offset
void CShield::Rotate(float angle)
{
	if(this->m_fOffX > 3.1f || this->m_fOffZ < 1.2f)
	{	
		this->m_bIncrementUp = false;
	}
	else if(this->m_fOffX < 1.1f || this->m_fOffZ > 3.1f)
	{	
		this->m_bIncrementUp = true;
	}
	
	if(this->m_bIncrementUp)
	{
		this->m_fOffX += abs(angle) * CShield::InputRotateRate / 2.45;
		this->m_fOffZ -= abs(angle) * CShield::InputRotateRate / 2.45;
	}
	else
	{
		this->m_fOffX -= abs(angle) * CShield::InputRotateRate / 2.45;
		this->m_fOffZ += abs(angle) * CShield::InputRotateRate / 2.45;
	}

	this->m_fShieldRotation += angle;
}

void CShield::Set_Color(int color)
{
	this->m_iColor = color;
}

#pragma endregion

#pragma region Static Methods
// build out the mesh to display the texture for the bullet
void CShield::BuildMesh(CGraphicsDevice* device)
{
	if(CShield::g_pMesh != NULL)
		delete CShield::g_pMesh;

	CShield::g_pMesh = new CMesh(device);
	float scaleX = 1.0f;
	float scaleY = 3.0f;

	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -scaleX;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -scaleY;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = scaleX;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -scaleY;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -scaleX;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = scaleY;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = scaleX;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = scaleY;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	CShield::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CShield::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CShield::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CShield::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion

#pragma region ICollidable Methods
// check for collisions; shields don't collide with objects as a recipient.
bool CShield::CollisionOccured(ICollidableUnit* unit)
{
	CBullet* bullet = dynamic_cast<CBullet*>(unit);

	//Check for explosion status
	if(bullet != NULL && bullet->Get_Owner() != BulletOwner::Owner_Ship)
	{
		CAnimation* explosion = (CAnimation*)CShield::g_pSceneManager->Create(GameObject::Explosion);
		explosion->Set_Position(bullet->Get_Position());
		explosion->Start();
		return true;
	}
	return false;
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the shield object
void CShield::Initialize()
{
	CGameUnit::Initialize();
}

// load the core components of the object
void CShield::LoadContent()
{
	CGameUnit::LoadContent();

	// initial gathering of all the services
	//this->m_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	
	if(CShield::g_pCollision == NULL)
		CShield::g_pCollision = (ICollisionService*)this->Get_Game()->Get_Services()->GetService("ICollisionService");

	// loading of the effects, textures, and mesh
	if(CShield::g_pCamera == NULL)
		CShield::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CShield::g_pEffect == NULL)
		CShield::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\ShieldEffect.fx");
	if(CShield::g_pTexture == NULL)
		CShield::g_pTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\Shield.png");
	if(CShield::g_pNoiseTexture == NULL)
		CShield::g_pNoiseTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\NoiseTexture.png");
	if(CShield::g_pShieldMask == NULL)
		CShield::g_pShieldMask = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\shieldMask.png");
	if(CShield::g_pSceneManager == NULL)
		CShield::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	if(CShield::g_pMesh == NULL)
		CShield::BuildMesh(this->Get_Game()->Get_GraphicsDevice());

	if(CShield::g_pCamera == NULL)
		exit(-1);
}

//update the shield's world position
void CShield::Update(CGameTime* gameTime)
{
	CGameUnit::Update(gameTime);

	D3DXMatrixIdentity(this->m_pWorld);
	
	//apply a tiny translation to get the shield outside the ship a little bit
	D3DXMatrixTranslation(this->m_pTranslation, 5.3f, 0, 0);
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(0, 1, 0), this->m_fShieldRotation);
	
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);

	this->m_fNoiseX += 0.0005;

	if(this->m_fNoiseX > 1)
		this->m_fNoiseX = 0;

	//Get the angle of rotation of the shield
	//Set the bounding box of the shield
	D3DXVECTOR3 position;
	position.x = this->m_pWorld->_41;
	position.y = 0;
	position.z = this->m_pWorld->_43;

	//derive the pMin
	D3DXVECTOR3 min;
	min.x = position.x - this->m_fOffX;
	min.y = 0;
	min.z = position.z - this->m_fOffZ;

	//derive the pMax
	D3DXVECTOR3 max;
	max.x = position.x + this->m_fOffX;
	max.y = 0;
	max.z = position.z + this->m_fOffZ;

	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &min, sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &max, sizeof(D3DXVECTOR3));
}

// draw the shield out to the screen
void CShield::Draw(CGameTime* gameTime)
{
	CGameUnit::Draw(gameTime);

	// set vector declaration
	this->Get_Game()->Get_GraphicsDevice()->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	// set the basic effect parameters
	CShield::g_pEffect->Set_View(&CShield::g_pCamera->Get_View());
	CShield::g_pEffect->Set_World(this->m_pWorld);
	CShield::g_pEffect->Set_Projection(&CShield::g_pCamera->Get_Projection());

	// set the texture if one exists
	if(CShield::g_pTexture != NULL)
		CShield::g_pEffect->SetVariable("InputTexture", CShield::g_pTexture);

	if(CShield::g_pNoiseTexture != NULL)
		CShield::g_pEffect->SetVariable("NoiseTexture", CShield::g_pNoiseTexture);

	if(CShield::g_pShieldMask != NULL)
		CShield::g_pEffect->SetVariable("ShieldMask", CShield::g_pShieldMask);

	CShield::g_pEffect->SetVariable("NoiseX", this->m_fNoiseX);
	CShield::g_pEffect->SetVariable("ShieldType", this->m_iColor);

	CShield::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CShield::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			// draw the mesh
			technique->GetPassByIndex(p)->Apply(0);
			CShield::g_pMesh->Draw();
		}
	}
	CShield::g_pEffect->End();
}
#pragma endregion