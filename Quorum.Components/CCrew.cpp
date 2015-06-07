/**
 * @description 
 *   Crew member of the ship.  This is the only real unit that is controllable.
 *   This object implements the IBasicUnit interface to allow basic movement via
 *   the CBasicUnitController if needed.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CCrew::g_iReferences = 0;
CMesh* CCrew::g_pMesh = NULL;
CEffect* CCrew::g_pEffect = NULL;
CTexture2D* CCrew::g_pTexture = NULL;
ICameraService* CCrew::g_pCamera = NULL;

const float CCrew::InputMoveRate = 10;
const float CCrew::InputRotateRate = 10;
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor, takes in a ship object
CCrew::CCrew(CGame* game, CShip* ship, int identifier)
	: CGameUnit(game, identifier)
{
	//Declaring the variables
	CCrew::g_iReferences++;
	
	this->m_pShip = ship;
	this->m_pPosition = new D3DXVECTOR2(0,0);

	this->m_bBound = false;
	this->m_iStation = -1;

	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
}

//Deconstructor
CCrew::~CCrew()
{
	//De-incremement the number of references
	CCrew::g_iReferences--;

	//if there are no more object references present in the game, de-reference all the global variables
	if(CCrew::g_iReferences == 0)
	{
		delete CCrew::g_pMesh;
		CCrew::g_pMesh = NULL;

		delete CCrew::g_pEffect;
		CCrew::g_pEffect = NULL;

		delete CCrew::g_pTexture;
		CCrew::g_pTexture = NULL;

		if(this->m_pWorld != NULL)
			delete this->m_pWorld;

		if(this->m_pTranslation != NULL)
			delete this->m_pTranslation;

		if(this->m_pRotation != NULL)
			delete this->m_pRotation;
	}
}
#pragma endregion

#pragma region IBasicUnit Properties
//Get the movement rate
float CCrew::Get_MoveRate()
{
	return CCrew::InputMoveRate;
}

//Get the rotation rate
float CCrew::Get_RotateRate()
{
	return CCrew::InputRotateRate;
}

//Get the current position
D3DXVECTOR2 CCrew::Get_Position()
{
	return *this->m_pPosition;
}

//Set the position
void CCrew::Set_Position(D3DXVECTOR2 value)
{
	this->m_pPosition->x = 0;
	this->m_pPosition->y = 0;
	this->Move(value);
}

float CCrew::Get_Rotation()
{
	return 0.0f;
}

void CCrew::Set_Rotation(float value)
{
}
#pragma endregion

#pragma region IBasicUnit Methods
//Move the crew unit
void CCrew::Move(D3DXVECTOR2 value)
{
	//Adjust the crew's translation matrix by the offset
	(*this->m_pPosition) += value;
	::D3DXMatrixTranslation(this->m_pTranslation, this->m_pPosition->x, 0, this->m_pPosition->y);
}

//Rotate the crew unit
void CCrew::Rotate(float value)
{
	//Adjust the crew's rotation matrix based on the passed in value
	this->m_fRotation += value;
	::D3DXMatrixRotationY(this->m_pRotation, this->m_fRotation);
}

//Fire at the give target
void CCrew::Fire(D3DXVECTOR2 target)
{

}
#pragma endregion

#pragma region IControllableUnit Properties
//Get whether or not the crew unit is bound to a controller
bool CCrew::Get_IsBound()
{
	return this->m_bBound;
}

//Set the crew unit's bound property
void CCrew::Set_IsBound(bool value)
{
	this->m_bBound = value;
}
#pragma endregion

#pragma region ICollidable Methods
//Check to see if the crew collided with a passed in unit (right now does nothing)
bool CCrew::CollisionOccured(ICollidableUnit* unit)
{
	return false;
}
#pragma endregion

#pragma region Properies
//Return the ship the crew member is associated with
CShip* CCrew::Get_Ship()
{
	return this->m_pShip;
}

//Return the station that the crew member is bound to
CStation* CCrew::Get_BoundStation()
{
	return this->m_pShip->GetStationAt(this->m_iStation);
}

//Return the station that the crew member is bound to
int CCrew::Get_BoundStationIndex()
{
	return this->m_iStation;
}

//Return the station that the crew member is bound to
void CCrew::Set_BoundStationIndex(CStation* station)
{
	
}

//Return the station that the crew member is bound to
void CCrew::Set_BoundStationIndex(int index)
{
	this->m_iStation = index;
}

//Get whether or not the crew member is bound to a station
bool CCrew::Get_IsStationBound()
{
	return this->m_iStation > -1;
}

//Get the crew unit's world transform
D3DXMATRIX* CCrew::Get_Transform()
{
	return this->m_pWorld;
}

//Set the crew unit's world transform
void CCrew::Set_Transform(D3DXMATRIX* value)
{
	this->m_pWorld = value;
}
#pragma endregion

#pragma region Static Methods
//Builds a static mesh for the crew class
void CCrew::BuildMesh(CGraphicsDevice* device)
{
	if(CCrew::g_pMesh != NULL)
		delete CCrew::g_pMesh;

	//create a new mesh object for the crew member
	CCrew::g_pMesh = new CMesh(device);

	//Generate the vertices
	VertexPositionColorTexture vertices[4];
	vertices[0].Position.x = -0.25f;
	vertices[0].Position.y = 0;
	vertices[0].Position.z = -0.25f;
	vertices[0].TextureCoord.x = 0;
	vertices[0].TextureCoord.y = 0;

	vertices[1].Position.x = 0.25f;
	vertices[1].Position.y = 0;
	vertices[1].Position.z = -0.25f;
	vertices[1].TextureCoord.x = 1;
	vertices[1].TextureCoord.y = 0;

	vertices[2].Position.x = -0.25f;
	vertices[2].Position.y = 0;
	vertices[2].Position.z = 0.25f;
	vertices[2].TextureCoord.x = 0;
	vertices[2].TextureCoord.y = 1;

	vertices[3].Position.x = 0.25f;
	vertices[3].Position.y = 0;
	vertices[3].Position.z = 0.25f;
	vertices[3].TextureCoord.x = 1;
	vertices[3].TextureCoord.y = 1;

	//Set the mesh's vertices
	CCrew::g_pMesh->SetVertices((void*)vertices, 4);

	//Set the mesh's texture coordinates
	UINT indices[6] = {0,1,2,2,1,3};
	CCrew::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CCrew::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CCrew::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

}
#pragma endregion

#pragma region Overriden Methods
//Initialize the crew
void CCrew::Initialize()
{
	//Call CGameUnit's initialize
	CGameUnit::Initialize();

	//Get the camera service
	CCrew::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
	if(CCrew::g_pCamera == NULL)
		exit(-1);
}

//Load content associated to the crew member
void CCrew::LoadContent()
{
	CGameUnit::LoadContent();

	//Get the effect, mesh, and texture if the global properties are set to null
	if(CCrew::g_pEffect == NULL)
		CCrew::g_pEffect = CEffect::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Effects\\DefaultTextureEffect.fx");
	if(CCrew::g_pTexture == NULL)
		CCrew::g_pTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\crew_member.png");
	if(CCrew::g_pMesh == NULL)
		CCrew::BuildMesh(this->Get_Game()->Get_GraphicsDevice());
}

//Update the crew member's world transform
void CCrew::Update(CGameTime* gameTime)
{
	//Call CGameUnit's draw
	CGameUnit::Update(gameTime);

	// update the crew's position
	//D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
}

//Draw the crew member
void CCrew::Draw(CGameTime* gameTime)
{
	//Call CGameUnit's draw
	CGameUnit::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	//Set the effect's parameters
	CCrew::g_pEffect->Set_View(&CCrew::g_pCamera->Get_View());
	CCrew::g_pEffect->Set_World(this->m_pWorld);
	CCrew::g_pEffect->Set_Projection(&CCrew::g_pCamera->Get_Projection());

	if(CCrew::g_pTexture != NULL)
		CCrew::g_pEffect->SetVariable("InputTexture", CCrew::g_pTexture);

	//Pass through the effect's draw sequence
	CCrew::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CCrew::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CCrew::g_pMesh->Draw();
		}
	}
	CCrew::g_pEffect->End();
}
#pragma endregion

