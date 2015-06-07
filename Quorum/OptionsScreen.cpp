#include "Quorum.h"

OptionsScreen::OptionsScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

OptionsScreen::~OptionsScreen()
{

}

void OptionsScreen::Initialize()
{
	
}

void OptionsScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{

}

void OptionsScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{

}

void OptionsScreen::Update(CGameTime* gameTime)
{
	//if(m_bIsEnabled)
	//{
		if(this->m_pInputService->Get_Platform() == Platform::Windows)
			this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
		else
			this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);
	//}
}

void OptionsScreen::Draw(CGameTime* gameTime)
{

}

void OptionsScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;
}

void OptionsScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

#pragma region Overridden Methods
char* OptionsScreen::Get_Type()
{
	return "OptionsScreen";
}
#pragma endregion