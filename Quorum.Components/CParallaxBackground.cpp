/**
 * @description 
 *   This is a single layer of the parallaxing background;  essentially
 *   this is a list of tiles that will "walk" and "wrap" as the user moves
 *   its offset.  The class can have a dynamic number of tiles and tile size.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Properties

CGameTime* CParallaxBackground::g_pGameTime = NULL;

float CParallaxBackground::Get_MoveRate()
{
	return this->m_fMoveRate;
}

void CParallaxBackground::Get_MoveRate(float value)
{
	this->m_fMoveRate = value;
}
#pragma endregion  


#pragma region Constructors and Finalizers
CParallaxBackground::CParallaxBackground(CGame* game, D3DXVECTOR2 mapSize, float moveRate)
:CTiledPlane(game, mapSize)
{
	this->m_fMoveRate = moveRate;
}

CParallaxBackground::CParallaxBackground(CGame* game, D3DXVECTOR3 normal, D3DXVECTOR2 mapSize, float moveRate)
:CTiledPlane(game, normal, mapSize)
{
	this->m_fMoveRate = moveRate;
}

CParallaxBackground::CParallaxBackground(CGame* game, D3DXVECTOR2 mapSize, D3DXVECTOR2 tileSize, float moveRate)
:CTiledPlane(game, mapSize, tileSize)
{
	this->m_fMoveRate = moveRate;
}

CParallaxBackground::CParallaxBackground(CGame* game, D3DXVECTOR3 normal, D3DXVECTOR2 mapSize, D3DXVECTOR2 tileSize, float moveRate)
:CTiledPlane(game, normal, mapSize, tileSize, TileScrollMode::WrapAround)
{
	this->m_fMoveRate = moveRate;
}

CParallaxBackground::~CParallaxBackground()
{
	CTiledPlane::~CTiledPlane();
}
#pragma endregion

#pragma region Methods
void CParallaxBackground::Scroll(D3DXVECTOR2 value)
{
	D3DXVECTOR2 stick;
	stick = value / CParallaxBackground::g_pGameTime->Get_ElapsedTime();
	float length = D3DXVec2Length(&stick);
	CShip::engineNoise->SetPitch(XACTPITCH_MIN + abs(XACTPITCH_MAX_TOTAL*length*0.75));
	CShip::engineNoise->SetVolume(length*0.55);

	CTiledPlane::Scroll(value * this->m_fMoveRate);
}

void CParallaxBackground::ScrollTo(D3DXVECTOR2 value)
{
	CTiledPlane::ScrollTo(value * this->m_fMoveRate);
}

void CParallaxBackground::LoadMap(char* path)
{
	
}
#pragma endregion

#pragma region Overriden Methods
void CParallaxBackground::UpdateTileTextures()
{
	
}

void CParallaxBackground::Initialize()
{
	CTiledPlane::Initialize();

	IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
	this->m_pInputService = inputManager->GetInput(PlayerIndex::One);
	if(this->m_pInputService == NULL)
		exit(-1);
}

void CParallaxBackground::LoadContent()
{
	CTiledPlane::LoadContent();
}

void CParallaxBackground::Update(CGameTime* gameTime)
{
	CTiledPlane::Update(gameTime);

	if(CParallaxBackground::g_pGameTime == NULL)
		CParallaxBackground::g_pGameTime = gameTime;
}
#pragma endregion