/**
 * @description 
 *   Collection of the parallax backgrounds into a single object.  This
 *   class maintains the world offset so that items can draw on the canvas
 *   correctly.  Implements a game service so that other classes can hook
 *   onto the functionality.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Overriden Properties
void CWorldBackground::Set_Enabled(bool value)
{
	CDrawableGameComponent::Set_Enabled(value);
	for(int i = 0; i < this->m_pLayers.size(); i++)
		this->m_pLayers[i]->Set_Enabled(value);
}

void CWorldBackground::Set_Visible(bool value)
{
	CDrawableGameComponent::Set_Visible(value);
	for(int i = 0; i < this->m_pLayers.size(); i++)
		this->m_pLayers[i]->Set_Visible(value);
}
#pragma endregion

#pragma region Constructors and Finalizers
CWorldBackground::CWorldBackground(CGame* game)
:	CDrawableGameComponent(game)
{
	this->m_fSpeed = 0;
	this->m_sOffset = D3DXVECTOR2(0,0);
	game->Get_Services()->AddService(this);
}
CWorldBackground::~CWorldBackground()
{
}
#pragma endregion

#pragma region Methods
void CWorldBackground::AddLayer(CParallaxBackground* layer)
{
	this->m_fSpeed = max(this->m_fSpeed, layer->Get_MoveRate());
	this->m_pLayers.push_back(layer);
	this->Get_Game()->Get_Components()->Add(layer);
}
CParallaxBackground* CWorldBackground::GetLayer(int index)
{
	return this->m_pLayers[index];
}

int CWorldBackground::GetNumberOfLayers()
{
	return this->m_pLayers.size();
}
#pragma endregion

#pragma region IWorldScroller Methods
void CWorldBackground::Scroll(D3DXVECTOR2 value)
{
	this->m_sOffset += value * this->m_fSpeed;
	for(int i = 0; i < this->m_pLayers.size(); i++)
		this->m_pLayers[i]->Scroll(value);
}

void CWorldBackground::ScrollTo(D3DXVECTOR2 value)
{
	value -= this->m_sOffset;
	value /= this->m_fSpeed;
	this->Scroll(value);
}

D3DXVECTOR2 CWorldBackground::ToWorldCoord(D3DXVECTOR2 value)
{
	return value - this->m_sOffset;
}

D3DXVECTOR2 CWorldBackground::ToScreenCoord(D3DXVECTOR2 value)
{
	return value + this->m_sOffset;
}
#pragma endregion