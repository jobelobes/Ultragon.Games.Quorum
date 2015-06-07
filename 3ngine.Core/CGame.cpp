#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CGame::CGame()
{
	this->m_pTimer = new CGameTime();
	this->m_pGameWindow = new CGameWindow();
	this->m_pGraphicsDevice = new CGraphicsDevice(this->m_pGameWindow);
	this->m_pComponentManager = new CGameComponentManager(this);
	this->m_pServicesManager = new CServiceManager(this);
}
CGame::~CGame()
{
	if(this->m_pComponentManager != NULL)
		delete this->m_pComponentManager;
	if(this->m_pServicesManager != NULL)
		delete this->m_pServicesManager;
	if(this->m_pGameWindow != NULL)
		delete this->m_pGameWindow;
	if(this->m_pGraphicsDevice != NULL)
		delete this->m_pGraphicsDevice;
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
	this->m_pGraphicsDevice->Present();
}

void CGame::Update(CGameTime* gameTime)
{
	this->m_pComponentManager->Update(gameTime);
}

void CGame::BeginRun()
{
	this->m_pServicesManager->Initialize();
	this->m_pGameWindow->Show();
	this->m_pGraphicsDevice->Create();

	this->Initialize();
	this->m_pComponentManager->Initialize();

	this->LoadContent();
	this->m_pComponentManager->LoadContent();
}

void CGame::EndRun()
{
	this->m_pGraphicsDevice->Destroy();
	this->m_pGameWindow->Close();

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

			::ValidateRect(this->m_pGameWindow->Get_Handle(), NULL);
		}
	}

	this->EndRun();
}

void CGame::Exit()
{
	::PostMessage(this->m_pGameWindow->m_hWnd, WM_QUIT, 0, 0); 
}
#pragma endregion

#pragma region Properties
CGraphicsDevice* CGame::Get_GraphicsDevice()
{
	return this->m_pGraphicsDevice;
}

CGameComponentManager* CGame::Get_Components()
{
	return this->m_pComponentManager;
}

CServiceManager* CGame::Get_Services()
{
	return this->m_pServicesManager;
}

CGameWindow* CGame::Get_Window()
{
	return this->m_pGameWindow;
}

CInput* CGame::Get_Input()
{
	return this->m_pInput;
}
#pragma endregion