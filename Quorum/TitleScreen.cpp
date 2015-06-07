/**
 * @description 
 *   Defines the Title screen for the game. Controls what is drawn on the title screen
 *   and what menu options are available from the title screen and which screens can be 
 *   navigated to.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

TitleScreen::TitleScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
	this->m_pAnimation = NULL;
}

TitleScreen::~TitleScreen()
{

}

void TitleScreen::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	this->m_pSpriteBatch = (CSpriteBatch*)this->Get_Game()->Get_Services()->GetService("CSpriteBatch");

	CTexture2D* texture = texture->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\ArtAssets\\starbg.png");
	CSprite2D sprite;
	sprite.SetTexture(texture);

	D3DXMATRIX matScaling;
	D3DXMATRIX matTranslation;
	D3DXMatrixScaling(&matScaling, (float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width,
		(float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height, 1.0f);
	D3DXMatrixTranslation(&matTranslation, (float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		(float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 1.0f);
	sprite.SetTransform(matScaling*matTranslation);
	

	CTexture2D* texture2 = texture2->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\title.png");//ArtAssets\\titleScreen_noname.png");
	CSprite2D sprite2;
	sprite2.SetTexture(texture2);
	D3DXMatrixTranslation(&matTranslation, this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 0.9);
	sprite2.SetTransform(matScaling*matTranslation);
	sprites.push_back(sprite2);
	sprites.push_back(sprite);


	//Load Menu animation
	CTexture2D* menuTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\titleOptions.png");
	this->m_pAnimation = new CAnimation(this->Get_Game());
	this->m_pAnimation->Set_ImageSize(D3DXVECTOR2(142, 97));
	this->m_pAnimation->Set_TextureSize(D3DXVECTOR2(426, 97));
	this->m_pAnimation->Set_ImageScale(D3DXVECTOR2(10,7));
	this->m_pAnimation->Set_FramePerSecond(60);
	this->m_pAnimation->Set_NumberOfFrames(3);
	this->m_pAnimation->Set_Texture(menuTexture);
	this->m_pAnimation->Set_InitialFrame(0);
	this->m_pAnimation->Set_IsLooping(false);
	this->m_pAnimation->Set_Enabled(true);
	this->m_pAnimation->Set_Visible(true);
	this->m_pAnimation->Set_WaitOnFirstFrame(true);
	this->Get_Game()->Get_Components()->Add(this->m_pAnimation);
	this->m_pAnimation->Set_Position(D3DXVECTOR2(0.1,1.4));
	this->m_pAnimation->Start();
	this->m_pAnimation->Pause(true);


	//Load Menu
	m_sMenuEntries.push_back("New Game");
	m_sMenuEntries.push_back("Options");
	m_sMenuEntries.push_back("Credits");

	IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
	XACTINDEX introLoop;
	s->GetCueIndex("introLoop", &introLoop);
	s->PlaySound(introLoop);

}

void TitleScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	//Menu Entry changing
	if(inputService->IsPressed(DIK_W) || inputService->IsPressed(DIK_UPARROW))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
		{
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
		}
		this->m_pAnimation->Set_CurrentFrame(m_iSelectedEntry);

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);

	}
	if(inputService->IsPressed(DIK_S) || inputService->IsPressed(DIK_DOWNARROW))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
		{
			m_iSelectedEntry = 0;
		}
		this->m_pAnimation->Set_CurrentFrame(m_iSelectedEntry);

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);
	}

	//Handle menu selection and appropriate action here
	if(inputService->IsPressed(DIK_RETURN))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);

		//Go to new screen
		if(m_iSelectedEntry == 0)
		{
			LobbyScreen* sc = (LobbyScreen*)CGameScreen::g_pScreenManager->GetScreen("LobbyScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
		else if(m_iSelectedEntry == 1)
		{
			ControlsScreen* sc = (ControlsScreen*)CGameScreen::g_pScreenManager->GetScreen("ControlsScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
		else if(m_iSelectedEntry == 2)
		{
			CreditsScreen* sc = (CreditsScreen*)CGameScreen::g_pScreenManager->GetScreen("CreditsScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
	}

	if(inputService->IsPressed(DIK_ESCAPE))
		this->Get_Game()->Exit();

	//CMenuScreen::Update(gameTime);
}

void TitleScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_B))
		this->Get_Game()->Exit();

	//Menu Entry changing
	else if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_UP))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
		{
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
		}
		this->m_pAnimation->Set_CurrentFrame(m_iSelectedEntry);

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_DOWN))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
		{
			m_iSelectedEntry = 0;
		}
		this->m_pAnimation->Set_CurrentFrame(m_iSelectedEntry);

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);
	}

	//Handle menu selection and appropriate action here
	else if(inputService->IsPressed(XButtonState::GAMEPAD_A))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);

		if(m_iSelectedEntry == 0)
		{
			LobbyScreen* sc = (LobbyScreen*)CGameScreen::g_pScreenManager->GetScreen("LobbyScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
		else if(m_iSelectedEntry == 1)
		{
			ControlsScreen* sc = (ControlsScreen*)CGameScreen::g_pScreenManager->GetScreen("ControlsScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
		else if(m_iSelectedEntry == 2)
		{
			CreditsScreen* sc = (CreditsScreen*)CGameScreen::g_pScreenManager->GetScreen("CreditsScreen");
			sc->Activate(this);
			Deactivate();
			this->m_pAnimation->Set_Enabled(false);
			this->m_pAnimation->Set_Visible(false);
		}
	}

}

void TitleScreen::Update(CGameTime* gameTime)
{
	CGameScreen::Update(gameTime);

	if(m_iSelectedEntry == -1)
		m_iSelectedEntry = 0;
}

void TitleScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();

	m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);

	for(int i = 0; i < size; ++i)
		m_pSpriteBatch->DrawBuffered(sprites[i].GetSprite(), 1);

	m_pSpriteBatch->End();
}

void TitleScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);

	if(this->m_pAnimation != NULL)
	{
		this->m_pAnimation->Set_Enabled(true);
		this->m_pAnimation->Set_Visible(true);
	}
}

void TitleScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
}

#pragma region Overridden Methods
char* TitleScreen::Get_Type()
{
	return "TitleScreen";
}
#pragma endregion