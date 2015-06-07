#include "3ngine.Extensions.h"

#pragma region Constructors and Finalizers
CSceneManager::CSceneManager(CGame* game)
: CDrawableGameComponent(game)
{
	game->Get_Services()->AddService(this);
}

CSceneManager::~CSceneManager()
{

}
#pragma endregion

#pragma region ISceneManager Properties
IScene* CSceneManager::Get_CurrentScene()
{
	return NULL;
}
#pragma endregion

#pragma region ISceneManager Methods
CDrawableGameComponent** CSceneManager::GetGameComponents(CBoundingBox* bbox, int* size)
{
	(*size) = 0;

	CGameComponentManager* mgr = this->Get_Game()->Get_Components();
	int count = mgr->Get_Count();

	CDrawableGameComponent** components = NULL;
	if(count > 0)
	{
		components = new CDrawableGameComponent*[count];
		for(int i = 0; i < count; i++)
		{
			CDrawableGameComponent* obj = dynamic_cast<CDrawableGameComponent*>((*mgr)[i]);
			if(obj != NULL && obj->Get_Visible() && obj->Get_Enabled())
				components[(*size)++] = obj;
		}
	}	
	return components;
}
#pragma endregion