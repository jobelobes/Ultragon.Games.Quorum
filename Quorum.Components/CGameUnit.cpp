/**
 * @description 
 *   Basic underlying class for most of the objects in the game.  Implements bounding box drawing
 *   ,basic attribute maintainence and collision detection callback.
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
ICameraService* CGameUnit::g_pCamera = NULL;
IWorldScroller* CGameUnit::g_pWorldScroller = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
//Default constructor
CGameUnit::CGameUnit(CGame* game, int identifier) 
	: CDrawableGameComponent(game) 
{
	//Unit attributes that will be set later
	this->m_pHp = NULL;
	this->m_pMaxHP = NULL;
	this->m_pAttackRadius = NULL;
	this->m_pDefensePower = NULL;
	this->m_pAttackRadius = NULL;
	this->m_pBoundingBox = new CBoundingBox(D3DXVECTOR3(-1,0,-1), D3DXVECTOR3(1,0,1));
	this->m_pBoundingPolygon = NULL;

	this->m_iIdentifier = identifier;
}

//Deconstructor
CGameUnit::~CGameUnit() 
{
	//Reset the bounding box
	if(this->m_pBoundingBox != NULL)
		delete this->m_pBoundingBox;
	if(this->m_pBoundingPolygon != NULL)
		delete this->m_pBoundingPolygon;
}
#pragma endregion

#pragma region Properties
//Get the unit's HP
int CGameUnit::Get_HP()
{
	return this->m_pHp;
}

//Set the unit's HP
void CGameUnit::Set_HP(int value)
{
	this->m_pHp = value;
}

//Get the unit's max hp
int CGameUnit::Get_MaxHP()
{
	return this->m_pMaxHP;
}

//set the unit's max HP
void CGameUnit::Set_MaxHP(int value)
{
	this->m_pMaxHP = value;
}

//Get the unit's attack power
int CGameUnit::Get_AttackPower()
{
	return this->m_pAttackPower;
}

//Set the unit's attack power
void CGameUnit::Set_AttackPower(int value)
{
	this->m_pAttackPower = value;
}

//Get the unit's defense power
int CGameUnit::Get_DefensePower()
{
	return this->m_pDefensePower;
}

//Set the unit's defense power
void CGameUnit::Set_DefensePower(int value)
{
	this->m_pDefensePower = value;
}

//Get the unit's attack radius
int CGameUnit::Get_AttackRadius()
{
	return this->m_pAttackRadius;
}

int CGameUnit::Get_Identifier()
{
	return this->m_iIdentifier;
}

void CGameUnit::Set_Identifier(int value)
{
	this->m_iIdentifier = value;
}

ICameraService* CGameUnit::Get_Camera()
{
	return CGameUnit::g_pCamera;
}
#pragma endregion

#pragma region ICollidableUnit Properties
//Return the bounding box of the unit
CBoundingBox* CGameUnit::Get_BoundingBox()
{
	return this->m_pBoundingBox;
}
#pragma endregion

#pragma region IAdvancedCollidableUnit Properties
//Return the bounding polygon of the unit
CBoundingPolygon* CGameUnit::Get_BoundingPolygon()
{
	return this->m_pBoundingPolygon;
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the unit
void CGameUnit::Initialize()
{
	//Call CGameComponent's initialize
	CGameComponent::Initialize();

	//Retrieve the camera service from the game
	if(CGameUnit::g_pCamera == NULL)
		CGameUnit::g_pCamera = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");

	if(CGameUnit::g_pWorldScroller == NULL)
		CGameUnit::g_pWorldScroller = (IWorldScroller*)this->Get_Game()->Get_Services()->GetService("IWorldScroller");
}
//Draw the unit
void CGameUnit::Draw(CGameTime* gameTime)
{
	//DrawableGameComponent's draw
	CDrawableGameComponent::Draw(gameTime);

	////in in debug mode, draw the bounding box's
	//#ifdef _DEBUG
	//if(this->m_pBoundingPolygon != NULL)
	//	this->m_pBoundingPolygon->Draw(this->Get_Game()->Get_GraphicsDevice(), CGameUnit::g_pCamera);
	//else if(this->m_pBoundingBox != NULL)
	//{
	//	D3DXVECTOR2 min;
	//	memcpy(&min, &CGameUnit::g_pWorldScroller->ToScreenCoord(D3DXVECTOR2(this->m_pBoundingBox->Min.x, this->m_pBoundingBox->Min.z)), sizeof(D3DXVECTOR2));

	//	D3DXVECTOR2 max;
	//	memcpy(&max, &CGameUnit::g_pWorldScroller->ToScreenCoord(D3DXVECTOR2(this->m_pBoundingBox->Max.x, this->m_pBoundingBox->Max.z)), sizeof(D3DXVECTOR2));

	//	CSquare::Draw(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.25f), D3DXVECTOR3(min.x, 0, min.y), D3DXVECTOR3(max.x, 0, max.y), this->Get_Game()->Get_GraphicsDevice(), CGameUnit::g_pCamera);
	//}
	//#endif
}
#pragma endregion