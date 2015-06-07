/**
 * @description 
 *   The ship is a composite of other objects, including turrets and stations.  Each
 *   child is in relation to the ship so that the movement and rotations are hiearchical.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
int CShip::g_iReferences = 0;

CMesh* CShip::g_pMesh = NULL;
CEffect* CShip::g_pEffect = NULL;
CTexture2D* CShip::g_pTexture = NULL;
IXACT3Wave* CShip::engineNoise = NULL;

ICollisionService* CShip::g_pCollision = NULL;
ICameraService* CShip::g_pCamera = NULL;
IWorldScroller* CShip::g_pWorldScroller = NULL;
ISceneManager* CShip::g_pSceneManager = NULL;

const float CShip::InputMoveRate = 1;
const float CShip::InputRotateRate = D3DX_PI / 4;
#pragma endregion

#pragma region Properties
//Return the ship's world transform
D3DXMATRIX* CShip::Get_Transform()
{
	return this->m_pWorld;
}

//Return the world coords of the ship
D3DXVECTOR2 CShip::Get_WorldCoord()
{
	return CShip::g_pWorldScroller->ToWorldCoord(this->Get_Position());
}

bool CShip::Is_Breached()
{
	for(int i = 0; i < this->m_iStationCount; i++)
	{
		if(this->m_pStations[i]->m_sAttributes.Health == 0)
			return true;
	}
	return false;
}
#pragma endregion

#pragma region IBasicUnit Properties
//return the ship's movement rate
float CShip::Get_MoveRate()
{
	return CShip::InputMoveRate;
}

//return the ship's rotation rate
float CShip::Get_RotateRate()
{
	return CShip::InputRotateRate;
}

//Return the ship's position
D3DXVECTOR2 CShip::Get_Position()
{
	return this->m_sPosition;
}

//Set the ship's position
void CShip::Set_Position(D3DXVECTOR2 value)
{
	CShip::g_pWorldScroller->ScrollTo(-1 * value);
	this->m_sPosition = value;

	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 5, 0, this->m_sPosition.y - 5), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 5, 0, this->m_sPosition.y + 5), sizeof(D3DXVECTOR3));
}

float CShip::Get_Rotation()
{
	return this->m_fRotation;
}

void CShip::Set_Rotation(float value)
{
	this->m_fRotation = 0;
	this->Rotate(value);
}
#pragma endregion

#pragma region IControllableUnit Properties
//Return whether or not the ship is bound to a controller
bool CShip::Get_IsBound()
{
	return this->m_bBound;
}

//Set the ship's bound flag
void CShip::Set_IsBound(bool value)
{
	this->m_bBound = value;
}
#pragma endregion

#pragma region Overriden Properties
void CShip::Set_Enabled(bool value)
{
	CGameComponent::Set_Enabled(value);

	for(int i = 0; i < this->m_iStationCount; i++)
		this->m_pStations[i]->Set_Enabled(value);
}

void CShip::Set_Visible(bool value)
{
	CDrawableGameComponent::Set_Visible(value);

	for(int i = 0; i < this->m_iStationCount; i++)
		this->m_pStations[i]->Set_Visible(value);
}
#pragma endregion

#pragma region Constructors and Deconstructors
//Default constructor
CShip::CShip(CGame* game, int identifier) 
: CGameUnit(game, identifier)
{
	//Incremement the ship references
	CShip::g_iReferences++;

	this->m_bBound = false;

	//Declare the variables
	this->m_fRotation = 0.0f;
	ZeroMemory(this->m_sPosition, sizeof(D3DXVECTOR2));

	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 5, 0, this->m_sPosition.y - 5), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 5, 0, this->m_sPosition.y + 5), sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(this->m_pWorld = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pTranslation = new D3DXMATRIX());
	D3DXMatrixIdentity(this->m_pRotation = new D3DXMATRIX());
}

//Deconstructor
CShip::~CShip()
{
	//De-increment the ship references
	CShip::g_iReferences--;

	//If there are no more references, de-reference the global variables
	if(CShip::g_iReferences == 0)
	{
		delete CShip::g_pMesh;
		CShip::g_pMesh = NULL;

		delete CShip::g_pEffect;
		CShip::g_pEffect = NULL;

		delete CShip::g_pTexture;
		CShip::g_pTexture = NULL;
	}

	//de-reference the transformation matrices
	if(this->m_pWorld != NULL)
		delete this->m_pWorld;
	if(this->m_pStations != NULL)
		delete [] this->m_pStations;
}
#pragma endregion

#pragma region Methods
//Add a turret object to the ship at a given index. Currently does nothing
void CShip::AddTurret(int index, CTurret* turret)
{

}

//Return a turret given an index (if there is one)
CTurret* CShip::GetTurretAt(int index)
{
	if(this->m_pStations == NULL || index > this->m_iStationCount || index < 0)
		return NULL;

	return this->m_pStations[index]->Get_Turret();
}

//Return a station given an index (if there is one)
CStation* CShip::GetStationAt(int index)
{
	if(this->m_pStations == NULL || index > this->m_iStationCount || index < 0)
		return NULL;

	return this->m_pStations[index];
}

//Remove a turret from the ship. Currently does nothing
void CShip::RemoveTurret(int index)
{
	
}

//Build the station segments of the ship based on a max number of stations passed in
void CShip::BuildStations(int count)
{
	this->m_iStationCount = count;
	this->m_pStations = new CStation*[this->m_iStationCount];

	//load the game content objects
	std::vector<STATION*> stations;
	((CQuorumSceneManager*)CShip::g_pSceneManager)->Get_GameContent()->Get_Stations(stations);

	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	D3DXQUATERNION pOutRotation;

	//create the number of stations needed to surround the exterior of the ship
	for(int i = 0; i < this->m_iStationCount; i++)
	{
		if(i == 0)
		{
			//Adjust the station's transformation matrices before creating the object
			::D3DXMatrixTranslation(&translation, 0.5f, 0, 0);
			::D3DXMatrixRotationAxis(&rotation, &D3DXVECTOR3(0,1,0), 0);
		}
		else
		{
			//adjust the station's transform before creation

			float a = 2 * D3DX_PI / (this->m_iStationCount - 1) * i;
			::D3DXMatrixTranslation(&translation, 3.0f, 0, 0);
			::D3DXMatrixRotationAxis(&rotation, &D3DXVECTOR3(0,1,0), a);
		}

		StationAttributes tmp;
		tmp.Type = (StationType)stations[i]->type;
		tmp.ID = stations[i]->id;
		tmp.Health = stations[i]->health;
		tmp.Armor = stations[i]->armor;
		tmp.TurretType = stations[i]->turretType;

		//create the new station object
		this->m_pStations[i] = new CStation(this->Get_Game(), this, translation, rotation, tmp);
		this->m_pStations[i]->Set_Enabled(this->Get_Enabled());
		this->m_pStations[i]->Set_Visible(this->Get_Visible());
		this->Get_Game()->Get_Components()->Add(this->m_pStations[i]);
	}
}
#pragma endregion

#pragma region IBasicUnit Methods
//Move the ship based on a vector offset
void CShip::Move(D3DXVECTOR2 value)
{
	CShip::g_pWorldScroller->Scroll(value);

	this->m_sPosition = CShip::g_pWorldScroller->ToWorldCoord(D3DXVECTOR2(0,0));

	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 5, 0, this->m_sPosition.y - 5), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 5, 0, this->m_sPosition.y + 5), sizeof(D3DXVECTOR3));
}

//Rotate the ship based on an offset
void CShip::Rotate(float value)
{
	this->m_fRotation = this->m_fRotation + value;
	
	D3DXMatrixRotationAxis(this->m_pRotation, &D3DXVECTOR3(0,1,0), this->m_fRotation);
}

//Fire at the give target
void CShip::Fire(D3DXVECTOR2 target)
{

}
#pragma endregion

#pragma region Overridden Methods
//Check whether or not the ship collides with an object. Currently does nothing
bool CShip::CollisionOccured(ICollidableUnit* unit)
{
	//Check for explosion status
	CBullet* bullet = dynamic_cast<CBullet*>(unit);
	if(bullet != NULL && bullet->Get_Owner() != BulletOwner::Owner_Ship)
	{
		D3DXVECTOR2 diff = bullet->Get_Position() - this->m_sPosition;
		if(D3DXVec2Length(&diff) < 5)
		{
			D3DXVec2Normalize(&diff, &diff);
			float value = D3DXVec2Dot(&D3DXVECTOR2(1,0), &diff);
			float angle = acos(value);
			if (diff.y > 0) 
				angle = (2.0f * D3DX_PI) - angle;

			angle =  fmod((angle * 180 / D3DX_PI) - this->m_fRotation * 180 / D3DX_PI + 330, 360);
			int index = ((int)floor(angle / 60)) % 6 + 1;

			this->m_pStations[index]->m_sAttributes.Health -= bullet->Get_Damage();

			// Play an klaxon sound:
			IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
			XACTINDEX klaxon;
			s->GetCueIndex("klaxon", &klaxon);
			s->PlaySound(klaxon);

			diff = diff * 5;
			CAnimation* explosion = (CAnimation*)CShip::g_pSceneManager->Create(GameObject::Explosion);
			explosion->Set_Position(diff);
			explosion->Start();
			return true;
		}
	}

	CEnvironmentObject* envObj = dynamic_cast<CEnvironmentObject*>(unit);
	if(envObj != NULL)
	{
		return true;
	}

	return false;
}

//Initialize the ship object
void CShip::Initialize()
{
	CGameUnit::Initialize();

	//Load the static camera service
	if(CShip::g_pCamera == NULL)
		CShip::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");

	//Load the world scroller object
	if(CShip::g_pWorldScroller == NULL)
		CShip::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");

	if(CShip::g_pSceneManager == NULL)
		CShip::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");

	if(CShip::g_pWorldScroller == NULL)
		exit(-1);
}

//Load the content related to the ship
void CShip::LoadContent()
{
	CGameUnit::LoadContent();

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();

	//Create the mesh object
	if(CShip::g_pMesh == NULL)
		CShip::BuildMesh(device);

	//Load the effect file
	if(CShip::g_pEffect == NULL)
		CShip::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\ShipEffect.fx");

	//Create the texture
	if(CShip::g_pTexture == NULL)
		CShip::g_pTexture = CTexture2D::Load(device, "..\\Assets\\Art\\hull_floor.png");

	if(CShip::g_pCollision == NULL)
		CShip::g_pCollision = (ICollisionService*)this->Get_Game()->Get_Services()->GetService("ICollisionService");

	this->BuildStations(7);
}

//Update the ship's world transform
void CShip::Update(CGameTime* gameTime)
{
	CGameUnit::Update(gameTime);

	// update bounding box
	CBoundingBox* bbox = this->Get_BoundingBox();
	memcpy(&bbox->Min, &D3DXVECTOR3(this->m_sPosition.x - 5, 0, this->m_sPosition.y - 5), sizeof(D3DXVECTOR3));
	memcpy(&bbox->Max, &D3DXVECTOR3(this->m_sPosition.x + 5, 0, this->m_sPosition.y + 5), sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(this->m_pWorld);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pTranslation);
	D3DXMatrixMultiply(this->m_pWorld, this->m_pWorld, this->m_pRotation);
}

//Draw the ship
void CShip::Draw(CGameTime* gameTime)
{
	CGameUnit::Draw(gameTime);

	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);

	CShip::g_pEffect->Set_View(&CShip::g_pCamera->Get_View());
	CShip::g_pEffect->Set_World(this->m_pWorld);
	CShip::g_pEffect->Set_Projection(&CShip::g_pCamera->Get_Projection());
	if(CShip::g_pTexture != NULL)
		CShip::g_pEffect->SetVariable("Texture", CShip::g_pTexture);
	CShip::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CShip::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CShip::g_pMesh->Draw();
		}
	}
	CShip::g_pEffect->End();
}
#pragma endregion

#pragma region Static Methods
//Build the static mesh for the ship
void CShip::BuildMesh(CGraphicsDevice* device)
{
	float scale = 4.9f;
	CShip::g_pMesh = new CMesh(device);
	
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

	CShip::g_pMesh->SetVertices((void*)vertices, 4);

	UINT indices[6] = {0,1,2,2,1,3};
	CShip::g_pMesh->SetIndices((UINT*)indices, 6);

	UINT attributes[2] = {0,0};
	CShip::g_pMesh->SetAttributes((UINT*)attributes, 2);
	CShip::g_pMesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture::VertexElements, VertexPositionColorTexture::VertexElementCount);
}
#pragma endregion