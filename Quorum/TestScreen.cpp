#include "Quorum.h"

#pragma region Constructors and Finalizers
TestScreen::TestScreen(CGame* game)
: CGameScreen(game)
{
	
}

TestScreen::~TestScreen()
{

}
#pragma endregion

#pragma region Overriden Methods
void TestScreen::Initialize()
{
	CGameComponent::Initialize();

	this->Get_Game()->Get_Components()->Add(new CAxis(this->Get_Game()));

	IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
	this->m_pInputService = inputManager->GetInput(PlayerIndex::One);
}


void TestScreen::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	if(this->m_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)this->m_pInputService, gameTime);
	if(this->m_pInputService->Get_Platform() == Platform::Xbox360)
		this->HandleXboxInput((IXBoxInputService*)this->m_pInputService, gameTime);
}

char* TestScreen::Get_Type()
{
	return "TestScreen";
}
#pragma endregion

#pragma region Methods
void TestScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(DIK_TAB))
		this->Get_Game()->Get_GraphicsDevice()->ToggleFullscreen();

	if(inputService->IsPressed(DIK_ESCAPE))
		this->Get_Game()->Exit();
}

void TestScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_BACK))
		this->Get_Game()->Exit();
}

void TestScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
}

void TestScreen::Deactivate()
{

}
#pragma endregion