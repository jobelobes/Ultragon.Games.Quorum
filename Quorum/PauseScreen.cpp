#include "Quorum.h"

PauseScreen::PauseScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

PauseScreen::~PauseScreen()
{

}

void PauseScreen::Initialize()
{
	
}

void PauseScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{

}

void PauseScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{

}

void PauseScreen::Update(CGameTime* gameTime)
{
	//if(m_bIsEnabled)
	//{
		if(this->m_pInputService->Get_Platform() == Platform::Windows)
			this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
		else
			this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);
	//}
}

void PauseScreen::Draw(CGameTime* gameTime)
{

}

void PauseScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;
}

void PauseScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

#pragma region Overridden Methods
char* PauseScreen::Get_Type()
{
	return "PauseScreen";
}
#pragma endregion