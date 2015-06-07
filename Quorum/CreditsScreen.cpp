#include "Quorum.h"

CreditsScreen::CreditsScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

CreditsScreen::~CreditsScreen()
{

}

void CreditsScreen::Initialize()
{
	CGameComponent::Initialize();
}

void CreditsScreen::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	this->m_pSpriteBatch = (CSpriteBatch*)this->Get_Game()->Get_Services()->GetService("CSpriteBatch");

	IInputManager* inputManager = (IInputManager*)this->Get_Game()->Get_Services()->GetService("IInputManager");
	this->m_pInputService = inputManager->GetInput(PlayerIndex::One);
	//this->m_pScreenManager = (CScreenManager*)this->Get_Game()->Get_Services()->GetService("CScreenManager");

	CTexture2D* texture = texture->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\ArtAssets\\starbg.png");
	CSprite2D sprite;
	sprite.SetTexture(texture);

	D3DXMATRIX matScaling;
	D3DXMATRIX matTranslation;
	D3DXMatrixScaling(&matScaling, (float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width,
		(float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height, 1.0f);
	D3DXMatrixTranslation(&matTranslation, (float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		(float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 0.1f);
	sprite.SetTransform(matScaling*matTranslation);
	

	CTexture2D* texture2 = texture2->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\credits.png");
	CSprite2D sprite2;
	sprite2.SetTexture(texture2);
	D3DXMatrixTranslation(&matTranslation, this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 0.0);
	sprite2.SetTransform(matScaling*matTranslation);
	sprites.push_back(sprite2);
	sprites.push_back(sprite);
}

void CreditsScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(DIK_BACK))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_pParentScreen != NULL)
		{
			this->m_pParentScreen->Activate(NULL);
			Deactivate();
		}
	}	
}

void CreditsScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_B))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_pParentScreen != NULL)
		{
			this->m_pParentScreen->Activate(NULL);
			Deactivate();
		}
	}
}

void CreditsScreen::Update(CGameTime* gameTime)
{
	//if(m_bIsEnabled)
	//{
		if(this->m_pInputService->Get_Platform() == Platform::Windows)
			this->HandleWindowsInput((IWindowsInputService*)CGameScreen::g_pInputService, gameTime);
		else
			this->HandleXboxInput((IXBoxInputService*)CGameScreen::g_pInputService, gameTime);

		if(m_iSelectedEntry == -1)
			m_iSelectedEntry = 0;
	//}
}

void CreditsScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();

	m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);// | D3DX10_SPRITE_SORT_TEXTURE);

	for(int i = 0; i < size; ++i)
	{
		m_pSpriteBatch->DrawBuffered(sprites[i].GetSprite(), 1);
	}
	m_pSpriteBatch->End();
}

void CreditsScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;

	m_iSelectedEntry = -1;
}

void CreditsScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

#pragma region Overridden Methods
char* CreditsScreen::Get_Type()
{
	return "CreditsScreen";
}
#pragma endregion