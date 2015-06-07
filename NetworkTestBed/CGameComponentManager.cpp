#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CGameComponentManager::CGameComponentManager(CGame* game)
{
	this->m_pGame = game;
	this->m_eState = ComponentManagerState::Created;
}
#pragma endregion

#pragma region Properties
int CGameComponentManager::Get_Count()
{
	return this->m_lComponents.size();
}
#pragma endregion

#pragma region Methods
void CGameComponentManager::Clear()
{
	this->m_lComponents.clear();
}

void CGameComponentManager::Add(CGameComponent* component)
{
	this->m_lComponents.push_back(component);

	if(this->m_eState >= ComponentManagerState::Initialized)
		component->Initialize();

	if(this->m_eState >= ComponentManagerState::LoadedContent)
	{
		CDrawableGameComponent* drawableComponent = dynamic_cast<CDrawableGameComponent*>(component);
		if(drawableComponent != NULL)
			drawableComponent->LoadContent();
	}
}

void CGameComponentManager::Remove(CGameComponent* component)
{
	
}

void CGameComponentManager::Initialize()
{
	this->m_eState = ComponentManagerState::Initialized;

	size_t size = this->m_lComponents.size();
	for(unsigned int i = 0; i < size; i++)
		this->m_lComponents[i]->Initialize();
}

void CGameComponentManager::LoadContent()
{
	this->m_eState = ComponentManagerState::LoadedContent;

	size_t size = this->m_lComponents.size();
	for(unsigned int i = 0; i < size; i++)
	{
		CDrawableGameComponent* component = dynamic_cast<CDrawableGameComponent*>(this->m_lComponents[i]);
		if(component != NULL)
			component->LoadContent();
	}
}

void CGameComponentManager::Update(CGameTime* gameTime)
{
	this->m_eState = ComponentManagerState::Updated;

	size_t size = this->m_lComponents.size();
	for(unsigned int i = 0; i < size; i++)
		this->m_lComponents[i]->InternalUpdate(gameTime);
}

void CGameComponentManager::Draw(CGameTime* gameTime)
{
	this->m_eState = ComponentManagerState::Drawn;

	size_t size = this->m_lComponents.size();
	for(unsigned int i = 0; i < size; i++)
	{
		CDrawableGameComponent* component = dynamic_cast<CDrawableGameComponent*>(this->m_lComponents[i]);
		if(component != NULL)
			component->InternalDraw(gameTime);
	}
}
#pragma endregion

#pragma region Operator Overriden Methods
CGameComponent* CGameComponentManager::operator[] (const int index)
{
	return this->m_lComponents[index];
}
#pragma endregion