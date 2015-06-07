#include "3ngine.Core.h"

#pragma region Methods
void CGame1::Initialize()
{
	//printf("CGame.Initialize\n");
	CGame::Initialize();

	this->Get_Components()->Add(new CGameNetworkManager(this, "224.0.22.1", "5467", "@id/42"));
}

void CGame1::LoadContent()
{
	//printf("CGame.LoadContent\n");
	CGame::LoadContent();
}

void CGame1::Update(CGameTime* gameTime)
{
	//printf("CGame.Update\n");
	CGame::Update(gameTime);
}

void CGame1::Draw(CGameTime* gameTime)
{
	//printf("CGame.Draw\n");
	CGame::Draw(gameTime);
}
#pragma endregion