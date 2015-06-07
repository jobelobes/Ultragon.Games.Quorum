#include "Quorum.h"

VolumeOptionsScreen::VolumeOptionsScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

VolumeOptionsScreen::~VolumeOptionsScreen()
{

}

void VolumeOptionsScreen::Initialize()
{
	
}

void VolumeOptionsScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{

}

void VolumeOptionsScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{

}

void VolumeOptionsScreen::Update(CGameTime* gameTime)
{
	//if(m_bIsEnabled)
	//{
		if(this->m_pInputService->Get_Platform() == Platform::Windows)
			this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
		else
			this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);
	//}
}

void VolumeOptionsScreen::Draw(CGameTime* gameTime)
{

}

void VolumeOptionsScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;
}

void VolumeOptionsScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

#pragma region Overridden Methods
char* VolumeOptionsScreen::Get_Type()
{
	return "VolumeOptionsScreen";
}
#pragma endregion