#include "Quorum.h"

VideoOptionsScreen::VideoOptionsScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

VideoOptionsScreen::~VideoOptionsScreen()
{

}

void VideoOptionsScreen::Initialize()
{
	
}

void VideoOptionsScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{

}

void VideoOptionsScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{

}

void VideoOptionsScreen::Update(CGameTime* gameTime)
{
	//if(m_bIsEnabled)
	//{
		if(this->m_pInputService->Get_Platform() == Platform::Windows)
			this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
		else
			this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);
	//}
}

void VideoOptionsScreen::Draw(CGameTime* gameTime)
{

}

void VideoOptionsScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;
}

void VideoOptionsScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

#pragma region Overridden Methods
char* VideoOptionsScreen::Get_Type()
{
	return "VideoOptionsScreen";
}
#pragma endregion