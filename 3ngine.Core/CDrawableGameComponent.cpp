#include "3ngine.Core.h"

#pragma region Constructors and Deconstructors
CDrawableGameComponent::CDrawableGameComponent(CGame* game)
: CGameComponent(game){}

CDrawableGameComponent::~CDrawableGameComponent(){}
#pragma endregion

#pragma region Properties
bool CDrawableGameComponent::Get_Visible()
{
	return this->m_bVisible;
}
void CDrawableGameComponent::Set_Visible(bool flag)
{
	this->m_bVisible = flag;
}
#pragma endregion

#pragma region Methods
void CDrawableGameComponent::LoadContent()
{

}

void CDrawableGameComponent::UnloadContent()
{

}

void CDrawableGameComponent::InternalDraw(CGameTime* gameTime)
{
	if(!this->m_bVisible)
		return;

	this->Draw(gameTime);
}

void CDrawableGameComponent::Draw(CGameTime* gameTime)
{

}
#pragma endregion