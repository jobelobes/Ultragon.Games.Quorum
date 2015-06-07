#include "3ngine.Core.h"

#pragma region Constructors and Deconstructors
CGameComponent::CGameComponent(CGame* game)
{
	this->m_pGame = game;
	this->m_bEnabled = true;
}
CGameComponent::~CGameComponent()
{
	this->m_pGame = NULL;
}
#pragma endregion

#pragma region Methods
void CGameComponent::Initialize()
{
	
}

void CGameComponent::InternalUpdate(CGameTime* gameTime)
{
	if(!this->m_bEnabled)
		return;

	this->Update(gameTime);
}

void CGameComponent::Update(CGameTime* gameTime)
{

}
#pragma endregion

#pragma region Protected Properties
CGame* CGameComponent::Get_Game()
{
	return this->m_pGame;
}
#pragma endregion

#pragma region Public Properties
bool CGameComponent::Get_Enabled()
{
	return this->m_bEnabled;
}
void CGameComponent::Set_Enabled(bool flag)
{
	this->m_bEnabled = flag;
}
#pragma endregion