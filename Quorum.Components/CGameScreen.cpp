/**
 * @description 
 *   Controls what is drawn for a screen; this class is a base class that all
 *   other screens use to extend how the screen is displayed and how to navigate
 *   to other screens.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
IInputService* CGameScreen::g_pInputService = NULL;
IScreenManager* CGameScreen::g_pScreenManager = NULL;
ICameraService* CGameScreen::g_pCameraService = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CGameScreen::CGameScreen(CGame* game)
: CDrawableGameComponent(game)
{
}

CGameScreen::~CGameScreen()
{
}
#pragma endregion

#pragma region Methods
void CGameScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{

}

void CGameScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{

}

char* CGameScreen::Get_Type()
{
	return "CGameScreen";
}

void CGameScreen::Activate(CGameScreen* screen)
{

}

void CGameScreen::Deactivate()
{

}
#pragma endregion

#pragma region Overriden Methods
void CGameScreen::Initialize()
{
	CDrawableGameComponent::Initialize();

	if(CGameScreen::g_pInputService == NULL)
	{
		IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
		CGameScreen::g_pInputService = inputManager->GetInput(PlayerIndex::One);
	}

	if(CGameScreen::g_pScreenManager == NULL)
		CGameScreen::g_pScreenManager = (IScreenManager*)this->Get_Game()->Get_Services()->GetService("IScreenManager");

	if(CGameScreen::g_pCameraService == NULL)
		CGameScreen::g_pCameraService = (ICameraService*)this->Get_Game()->Get_Services()->GetService("ICameraService");
}

void CGameScreen::Update(CGameTime* gameTime)
{
	CDrawableGameComponent::Update(gameTime);

	if(CGameScreen::g_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)CGameScreen::g_pInputService, gameTime);
	else
		this->HandleXboxInput((IXBoxInputService*)CGameScreen::g_pInputService, gameTime);
}
#pragma endregion