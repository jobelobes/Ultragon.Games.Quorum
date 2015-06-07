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
int CAsteroid::g_iReferences = 0;

CMesh* CAsteroid::g_pMesh = NULL;
CEffect* CAsteroid::g_pEffect = NULL;
CTexture2D* CAsteroid::g_pTexture = NULL;
CTexture2D* CAsteroid::g_pNormalTexture = NULL;

ICollisionService* CAsteroid::g_pCollision = NULL;
ICameraService* CAsteroid::g_pCamera = NULL;
ISceneManager* CAsteroid::g_pSceneManager = NULL;
IWorldScroller* CAsteroid::g_pWorldScroller = NULL;
#pragma endregion

#pragma region Properties
//Return the asteroid's position
D3DXVECTOR3* CAsteroid::Get_Position()
{
	return this->m_pRelPosition;
}

//Set the position
void CAsteroid::Set_Position(D3DXVECTOR2* position)
{
	this->m_pRelPosition = &D3DXVECTOR3(position->x, 0, position->y);
}
#pragma endregion

#pragma region ICollidable Properties
//Return the bounding box of the ICollidableUnit
CBoundingBox* CAsteroid::Get_BoundingBox()
{
	return this->m_pBoundingBox;
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CAsteroid::CAsteroid(CGame* game)
	: CEnvironmentObject(game) 
{
	//Declare the variables
	CAsteroid::g_iReferences++;

	this->Set_Enabled(false);
	this->Set_Visible(false);
	this->m_pRelPosition = new D3DXVECTOR3(0,0,0);
	this->m_pVelocity = new D3DXVECTOR3(0,0,0);
	this->m_iObjectType = EnvironmentObjectType::Asteroid;

	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());
}

//Deconstructor
CAsteroid::~CAsteroid()
{
	//De-incremement the number of object references for the bullet objects
	CAsteroid::g_iReferences--;

	//If there are no more bullet objects initialized, then de-reference the global variables
	if(CAsteroid::g_iReferences == 0)
	{
		delete CAsteroid::g_pMesh;
		CAsteroid::g_pMesh = NULL;

		delete CAsteroid::g_pEffect;
		CAsteroid::g_pEffect = NULL;

		delete CAsteroid::g_pTexture;
		CAsteroid::g_pTexture = NULL;

		delete CAsteroid::g_pNormalTexture;
		CAsteroid::g_pNormalTexture = NULL;
	}

	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
	if(this->m_pRelPosition != NULL)
		delete this->m_pRelPosition;
	if(this->m_pVelocity != NULL)
		delete this->m_pVelocity;
}
#pragma endregion

#pragma region Methods

#pragma endregion

#pragma region ICollidable Methods
// check for collisions; bullets don't collide with objects as a recipient.
bool CAsteroid::CollisionOccured(ICollidableUnit* unit)
{
	return false;
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the turret object
void CAsteroid::Initialize()
{
	CDrawableGameComponent::Initialize();

	this->InitializeAsteroidAnimation();

	if(this->m_pAnimatedAsteroid != NULL)
		this->m_pAnimatedAsteroid->Initialize();

}

// load the core components of the object
void CAsteroid::LoadContent()
{
	CEnvironmentObject::LoadContent();

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	if(CAsteroid::g_pCamera == NULL)
		CAsteroid::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CAsteroid::g_pSceneManager == NULL)
		CAsteroid::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	if(CAsteroid::g_pWorldScroller == NULL)
		CAsteroid::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");

	// loading of the effect
	if(CAsteroid::g_pEffect == NULL)
		CAsteroid::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultTextureEffect.fx");

	// load the mesh and texture locally
	if(CAsteroid::g_pMesh == NULL)
		CAsteroid::g_pMesh = CEnvironmentObject::Get_Mesh(EnvironmentObjectType::Asteroid);

	if(CAsteroid::g_pTexture == NULL)
		CAsteroid::g_pTexture = CEnvironmentObject::Get_Texture(EnvironmentObjectType::Asteroid);

	if(CAsteroid::g_pNormalTexture == NULL)
		CAsteroid::g_pNormalTexture = CEnvironmentObject::Get_Texture(EnvironmentObjectType::AsteroidNormal);

	if(this->m_pAnimatedAsteroid != NULL)
		this->m_pAnimatedAsteroid->LoadContent();

	
	this->m_pAnimatedAsteroid->Set_Texture(CAsteroid::g_pTexture);
}

// update the position and drawable position
void CAsteroid::Update(CGameTime* gameTime)
{
	CEnvironmentObject::Update(gameTime);


	// update position based off velocity
	(*this->m_pRelPosition) += (*this->m_pVelocity) * gameTime->Get_ElapsedTime();

	// get the world map offset and recalculate the bounding box; needed for collision detection
	/*D3DXVECTOR2 offset = CEnvironmentObject::g_pWorldScroller->Get_Offset();
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &(*this->m_pRelPosition + D3DXVECTOR3(-0.25f,0,-0.25f)), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &(*this->m_pRelPosition + D3DXVECTOR3(0.25f,0,0.25f)), sizeof(D3DXVECTOR3));*/

	// update the draw position/rotation of the object
	D3DXMatrixTranslation(this->m_pTranslation, this->m_pRelPosition->x, 0, this->m_pRelPosition->z);
	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);

	// if a collision system exists, then check and see if the bullet has collided, if so kill thyself
	/*if(CEnvironmentObject::g_pCollision != NULL && CEnvironmentObject::g_pCollision->HasCollided(this))
		CEnvironmentObject::g_pSceneManager->Destroy(GameObject::Bullet, this);*/
}

// draw the bullet out to the screen
void CAsteroid::Draw(CGameTime* gameTime)
{
	CDrawableGameComponent::Draw(gameTime);

	// set vector declaration
	this->Get_Game()->Get_GraphicsDevice()->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	this->m_pAnimatedAsteroid->Start(false);

	//// set the basic effect parameters
	//CAsteroid::g_pEffect->Set_View(&CAsteroid::g_pCamera->Get_View());
	//CAsteroid::g_pEffect->Set_World(this->m_pWorld);
	//CAsteroid::g_pEffect->Set_Projection(&CAsteroid::g_pCamera->Get_Projection());

	//// set the texture if one exists
	//if(CAsteroid::g_pTexture != NULL)
	//	CAsteroid::g_pEffect->SetVariable("InputTexture", CAsteroid::g_pTexture);

	//CAsteroid::g_pEffect->Begin();
	//{
	//	ID3D10EffectTechnique* technique = CAsteroid::g_pEffect->Get_CurrentTechnique();

	//	D3D10_TECHNIQUE_DESC td;
	//	technique->GetDesc(&td);
	//	for( UINT p = 0; p < td.Passes; ++p )
	//	{
	//		// draw the mesh
	//		technique->GetPassByIndex(p)->Apply(0);
	//		CAsteroid::g_pMesh->Draw();
	//	}
	//}
	//CAsteroid::g_pEffect->End();
}
#pragma endregion

//Create my m_pAnimatedAsteroid
void CAsteroid::InitializeAsteroidAnimation()
{
	this->m_pAnimatedAsteroid = new CAnimation(this->Get_Game());
	this->m_pAnimatedAsteroid->Set_ImageScale(D3DXVECTOR2(1,1));
	this->m_pAnimatedAsteroid->Set_ImageSize(D3DXVECTOR2(64,64));
	this->m_pAnimatedAsteroid->Set_TextureSize(D3DXVECTOR2(512,512));
	this->m_pAnimatedAsteroid->Set_Position(D3DXVECTOR2(0, 0));
//	this->m_pAnimatedAsteroid->Set_Texture(CTurret::g_pTextures[this->m_sAttributes.Type]);
	this->m_pAnimatedAsteroid->Set_FramePerSecond(8);
	this->m_pAnimatedAsteroid->Set_NumberOfFrames(64);
	this->m_pAnimatedAsteroid->Set_InitialFrame(0);
	this->m_pAnimatedAsteroid->Set_IsLooping(true);
	this->m_pAnimatedAsteroid->Set_WaitOnFirstFrame(false);
	this->Get_Game()->Get_Components()->Add(this->m_pAnimatedAsteroid);
}