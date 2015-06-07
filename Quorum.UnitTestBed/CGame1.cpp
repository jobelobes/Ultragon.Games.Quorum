#include "Quorum.h"


#pragma region Methods
void CGame1::Initialize()
{
	CGame::Initialize();

	this->m_pSpriteBatch = new CSpriteBatch(this);
	
	this->m_pInputService = new CWindowsInput(this);
	this->Get_Components()->Add(this->m_pInputService);
	this->Get_Components()->Add(new CQuorumCamera(this, 50));
	/*this->Get_Components()->Add(new CParallaxBackground(this, &D3DXVECTOR2(4,3), &D3DXVECTOR2(10,10), 10));
	this->Get_Components()->Add(new CParallaxBackground(this, &D3DXVECTOR2(8,6), &D3DXVECTOR2(5,5), 20));
	this->Get_Components()->Add(new CParallaxBackground(this, &D3DXVECTOR2(40,30), &D3DXVECTOR2(1,1), 30));*/

	
	//this->Get_Components()->Add(new CAxis(this));
}

void CGame1::Update(CGameTime* gameTime)
{
	CGame::Update(gameTime);

	if(this->m_pInputService->IsPressed(DIK_TAB))
		this->Get_GraphicsDevice()->ToggleFullscreen();
	POINT* pt = this->m_pInputService->Get_MousePosition();
	sprintf(this->m_sBuffer, "%.2f FPS\n Mouse Coord: {x: %d, y: %d}", 1.0 / gameTime->Get_ElapsedTime(),pt->x, pt->y);
}

void CGame1::Draw(CGameTime* gameTime)
{
	CGraphicsDevice* device = this->Get_GraphicsDevice();
	device->Clear(D3DXCOLOR(0,0,0,0));

	CGame::Draw(gameTime);

	RECT rect = {5, 5, 0, 0};
	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT | D3DX10_SPRITE_SORT_TEXTURE);
	this->m_pSpriteBatch->DrawString(this->m_sBuffer, &rect, D3DXCOLOR(1,1,1,1));
	this->m_pSpriteBatch->End();
}
#pragma endregion