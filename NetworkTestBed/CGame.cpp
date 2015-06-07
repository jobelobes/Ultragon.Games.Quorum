#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CGame::CGame()
{
	this->m_pTimer = new CGameTime();
	
	this->m_pComponentManager = new CGameComponentManager(this);
	this->m_pServicesManager = new CServiceManager(this);
}
CGame::~CGame()
{
	if(this->m_pComponentManager != NULL)
		delete this->m_pComponentManager;
	if(this->m_pServicesManager != NULL)
		delete this->m_pServicesManager;
}
#pragma endregion

#pragma region Methods
void CGame::Initialize()
{
}

void CGame::LoadContent()
{
}

bool CGame::BeginDraw()
{
	return true;
}

void CGame::Draw(CGameTime* gameTime)
{
	this->m_pComponentManager->Draw(this->m_pTimer);
}

void CGame::EndDraw()
{

}

void CGame::Update(CGameTime* gameTime)
{
	this->m_pComponentManager->Update(gameTime);
}

void CGame::BeginRun()
{
	this->m_pServicesManager->Initialize();

	this->Initialize();
	this->m_pComponentManager->Initialize();

	this->LoadContent();
	this->m_pComponentManager->LoadContent();
}

void CGame::EndRun()
{
	delete this->m_pComponentManager;
	delete this->m_pServicesManager;
}

void CGame::Run()
{
	MSG msg = {0};
	this->BeginRun();

	this->m_pTimer->Reset();
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->m_pTimer->Tick();
			this->Update(this->m_pTimer);
			if(this->BeginDraw())
			{
				this->Draw(this->m_pTimer);
				this->EndDraw();
			}
		}
	}

	this->EndRun();
}

void CGame::Exit()
{

}
#pragma endregion

#pragma region Properties
CGameComponentManager* CGame::Get_Components()
{
	return this->m_pComponentManager;
}

CServiceManager* CGame::Get_Services()
{
	return this->m_pServicesManager;
}
#pragma endregion