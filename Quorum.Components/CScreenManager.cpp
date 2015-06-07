/**
 * @description 
 *   Manages all screens that are created. Allows screens to be added, removed and 
 *	 retrieved so that all screens are maintained in one central location.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

#pragma region Constructors and Finalizers
CScreenManager::CScreenManager(CGame* game)
: CGameComponent(game)
{
	game->Get_Services()->AddService(this);
}

CScreenManager::~CScreenManager()
{
	
}
#pragma endregion

#pragma region Methods
void CScreenManager::AddScreen(CGameScreen* gameScreen)
{
	this->m_pGameScreens.push_back(gameScreen);
	this->Get_Game()->Get_Components()->Add(gameScreen);

	/*if(this->m_pGameScreens.size() == 1)
		gameScreen->Activate();
	else*/
		gameScreen->Deactivate();
}

void CScreenManager::RemoveScreen(CGameScreen* gameScreen)
{

}

CGameScreen* CScreenManager::GetScreen(char* screenName)
{
	std::vector<CGameScreen*>::iterator i;
	for (i = this->m_pGameScreens.begin() ; i < this->m_pGameScreens.end(); i++)
	{
		char* typeName = (*i)->Get_Type();
		if(strcmp(typeName, screenName) == 0)
			return *i;
	}
	return NULL;
}
#pragma endregion

#pragma region Overridden Methods
void CScreenManager::Initialize()
{
	CGameComponent::Initialize();
}

void CScreenManager::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);
}
#pragma endregion