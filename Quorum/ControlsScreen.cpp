#include "Quorum.h"

#pragma region Constructors and Finalizers
ControlsScreen::ControlsScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
}

ControlsScreen::~ControlsScreen()
{

}
#pragma endregion

#pragma region Overridden Methods
void ControlsScreen::Initialize()
{
	CGameComponent::Initialize();
}

void ControlsScreen::LoadContent()
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
	sprites.push_back(sprite);

	CTexture2D* texture2 = texture2->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\control_0.png");
	CSprite2D sprite2;
	sprite2.SetTexture(texture2);
	D3DXMatrixTranslation(&matTranslation, this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 0.0);
	sprite2.SetTransform(matScaling*matTranslation);
	controllerSprites.push_back(sprite2);

	CTexture2D* texture3 = texture3->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\control_1.png");
	CSprite2D sprite3;
	sprite3.SetTexture(texture3);
	sprite3.SetTransform(matScaling*matTranslation);
	controllerSprites.push_back(sprite3);

	CTexture2D* texture4 = texture4->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\control_2.png");
	CSprite2D sprite4;
	sprite4.SetTexture(texture4);
	sprite4.SetTransform(matScaling*matTranslation);
	controllerSprites.push_back(sprite4);

	CTexture2D* texture5 = texture5->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\control_3.png");
	CSprite2D sprite5;
	sprite5.SetTexture(texture5);
	sprite5.SetTransform(matScaling*matTranslation);
	controllerSprites.push_back(sprite5);
	

	//Load Menu
	m_sMenuEntries.push_back("Anytime");
	m_sMenuEntries.push_back("Pilot");
	m_sMenuEntries.push_back("Gunner");
	m_sMenuEntries.push_back("Shield");
}

void ControlsScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	//Menu Entry changing
	if(inputService->IsPressed(DIK_A) || inputService->IsPressed(DIK_LEFTARROW))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
		{
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
		}
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);
	}
	if(inputService->IsPressed(DIK_D) || inputService->IsPressed(DIK_RIGHTARROW))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
		{
			m_iSelectedEntry = 0;
		}
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);
	}

	//Handles moving back to main screen
	if(inputService->IsPressed(DIK_BACKSPACE) && this->m_pParentScreen != NULL)
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		this->m_pParentScreen->Activate(NULL);
		Deactivate();
	}	
}

void ControlsScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	//Menu Entry changing
	if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_LEFT))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
		{
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
		}
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_RIGHT))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
		{
			m_iSelectedEntry = 0;
		}
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);
	}

	//Handles moving back to main screen
	else if(inputService->IsPressed(XButtonState::GAMEPAD_B))
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

void ControlsScreen::Update(CGameTime* gameTime)
{
	if(this->m_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)CGameScreen::g_pInputService, gameTime);
	else
		this->HandleXboxInput((IXBoxInputService*)CGameScreen::g_pInputService, gameTime);

	if(m_iSelectedEntry == -1)
		m_iSelectedEntry = 0;
}

void ControlsScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();

	m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);// | D3DX10_SPRITE_SORT_TEXTURE);

	m_pSpriteBatch->DrawBuffered(controllerSprites[m_iSelectedEntry].GetSprite(), 1);

	for(int i = 0; i < size; ++i)
	{
		m_pSpriteBatch->DrawBuffered(sprites[i].GetSprite(), 1);
	}
	m_pSpriteBatch->End();
}

void ControlsScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
}

void ControlsScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
}


char* ControlsScreen::Get_Type()
{
	return "ControlsScreen";
}
#pragma endregion