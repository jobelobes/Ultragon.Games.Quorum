#include "3ngine.Core.h"

#pragma region Properties
int CServiceManager::Get_Count()
{
	return this->m_lComponents.size();
}
#pragma endregion

#pragma region Constructors and Finalizers
CServiceManager::CServiceManager(CGame* game)
{
	this->m_pGame = game;
}

CServiceManager::~CServiceManager()
{

}
#pragma endregion

#pragma region Overriden Methods
void CServiceManager::Initialize()
{

}
#pragma endregion

#pragma region Methods
void CServiceManager::Clear()
{
	this->m_lComponents.clear();
}

void CServiceManager::AddService(IGameService* service)
{
	this->m_lComponents.push_back(service);
}

void CServiceManager::RemoveService(IGameService* service)
{

}

IGameService* CServiceManager::GetService(char* serviceName)
{
	std::vector<IGameService*>::iterator i;
	for (i = this->m_lComponents.begin() ; i < this->m_lComponents.end(); i++)
	{
		char* typeName = (*i)->Get_Type();
		if(strcmp(typeName, serviceName) == 0)
			return *i;
	}
	return NULL;
}
#pragma endregion