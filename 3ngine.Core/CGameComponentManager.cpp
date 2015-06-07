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
	return this->m_lComponents.size() + this->m_lDrawableComponents.size();
}

std::vector<CGameComponent*>* CGameComponentManager::Get_GameComponents()
{
	return &this->m_lComponents;
}

std::vector<CDrawableGameComponent*>* CGameComponentManager::Get_DrawableGameComponents()
{
	return &this->m_lDrawableComponents;
}
#pragma endregion

#pragma region Methods
void CGameComponentManager::Clear()
{
	this->m_lComponents.clear();
	this->m_lDrawableComponents.clear();
}

void CGameComponentManager::Add(CGameComponent* component)
{
	CDrawableGameComponent* drawableComponent = dynamic_cast<CDrawableGameComponent*>(component);
	if(drawableComponent != NULL)
		this->m_lDrawableComponents.push_back(drawableComponent);
	else
		this->m_lComponents.push_back(component);
	
	if(this->m_eState >= ComponentManagerState::Initialized)
		component->Initialize();

	if(drawableComponent != NULL && this->m_eState >= ComponentManagerState::LoadedContent)
		drawableComponent->LoadContent();
}

void CGameComponentManager::Remove(CGameComponent* component)
{
	
}

void CGameComponentManager::Initialize()
{
	this->m_eState = ComponentManagerState::Initialized;

	int size = this->m_lComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lComponents[i]->Initialize();

	size = this->m_lDrawableComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lDrawableComponents[i]->Initialize();
}

void CGameComponentManager::LoadContent()
{
	this->m_eState = ComponentManagerState::LoadedContent;

	int size = this->m_lDrawableComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lDrawableComponents[i]->LoadContent();
}

void CGameComponentManager::Update(CGameTime* gameTime)
{
	this->m_eState = ComponentManagerState::Updated;

	int size = this->m_lComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lComponents[i]->InternalUpdate(gameTime);

	size = this->m_lDrawableComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lDrawableComponents[i]->InternalUpdate(gameTime);
}

void CGameComponentManager::Draw(CGameTime* gameTime)
{
	this->m_eState = ComponentManagerState::Drawn;

	int size = this->m_lDrawableComponents.size();
	for(int i = 0; i < size; i++)
		this->m_lDrawableComponents[i]->InternalDraw(gameTime);

}
#pragma endregion

#pragma region Operator Overriden Methods
CGameComponent* CGameComponentManager::operator[] (const int index)
{
	int size = this->m_lComponents.size();
	if(index >= size)
		return this->m_lDrawableComponents[index - size];
	return this->m_lComponents[index];
}
#pragma endregion