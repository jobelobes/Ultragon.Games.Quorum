#include "Quorum.h"

EndGameScreen::EndGameScreen(CGame* game)
: CGameScreen(game)
{
	m_iSelectedEntry = 0;
	m_bIsEnabled = false;
	m_dTimeToWin = -1.0;
	m_iNumEnemiesKilled = -1;
	m_bHasScored = false;
	m_dScore = -1.0;
	winMsg = NULL;
}

EndGameScreen::~EndGameScreen(){}

void EndGameScreen::SetTimeToWin(double t)
{
	this->m_dTimeToWin = t;
}

void EndGameScreen::SetNumEnemiesKilled(int n)
{
	this->m_iNumEnemiesKilled = n;
}

void EndGameScreen::Initialize()
{
	CGameComponent::Initialize();
}

void EndGameScreen::LoadContent()
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
	D3DXMatrixTranslation(&matTranslation, 640 / 2.0f, 480 / 2.0f, 0.1f);
	sprite.SetTransform(matScaling*matTranslation);
	

	CTexture2D* texture2 = texture2->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\GAME_WIN.png");
	CSprite2D sprite2;
	sprite2.SetTexture(texture2);
	D3DXMatrixTranslation(&matTranslation, 640 / 2.0f, 480 / 2.0f, 0.0);
	sprite2.SetTransform(matScaling*matTranslation);
	sprites.push_back(sprite2);
	sprites.push_back(sprite);

	CTexture2D* texture3 = texture3->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\GAME_OVER.png");
	CSprite2D sprite3;
	sprite3.SetTexture(texture3);
	D3DXMatrixTranslation(&matTranslation, 640 / 2.0f, 480 / 2.0f, 0.0);
	sprite3.SetTransform(matScaling*matTranslation);
	sprites.push_back(sprite3);

	//Load Menu
	m_sMenuEntries.push_back("Main Menu");
	m_sMenuEntries.push_back("Exit");

	//Create End Game Messages
}

void EndGameScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	//Menu Entry changing
	if(inputService->IsPressed(DIK_W) || inputService->IsPressed(DIK_UPARROW))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
	}
	if(inputService->IsPressed(DIK_S) || inputService->IsPressed(DIK_DOWNARROW))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
			m_iSelectedEntry = 0;
	}

	//Handle menu selection and appropriate action here
	if(inputService->IsPressed(DIK_RETURN))
	{
		//Go to new screen
		if(m_iSelectedEntry == 0)
		{
			TitleScreen* sc = (TitleScreen*)CGameScreen::g_pScreenManager->GetScreen("TitleScreen");
			sc->Activate(NULL);
			Deactivate();
		}
		else if (m_iSelectedEntry == 1)
		{
			this->Get_Game()->Exit();
		}
	}	
}

void EndGameScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	//Menu Entry changing
	if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_UP))
	{
		m_iSelectedEntry--;
		if(m_iSelectedEntry < 0)
			m_iSelectedEntry = m_sMenuEntries.size() - 1;
	}
	if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_DOWN))
	{
		m_iSelectedEntry++;
		if(m_iSelectedEntry > m_sMenuEntries.size() - 1)
			m_iSelectedEntry = 0;
	}

	//Handle menu selection and appropriate action here
	if(inputService->IsPressed(XButtonState::GAMEPAD_A))
	{
		//Go to new screen
		if(m_iSelectedEntry == 0)
		{
			TitleScreen* sc = (TitleScreen*)CGameScreen::g_pScreenManager->GetScreen("TitleScreen");
			sc->Activate(NULL);
			Deactivate();
		}
		else if (m_iSelectedEntry == 1)
		{
			this->Get_Game()->Exit();
		}
	}
}

void EndGameScreen::Update(CGameTime* gameTime)
{
	if(!this->m_bHasScored)
	{
		CalculateScore();
	}

	if(this->m_pInputService->Get_Platform() == Platform::Windows)
		this->HandleWindowsInput((IWindowsInputService*)CGameScreen::g_pInputService, gameTime);
	else
		this->HandleXboxInput((IXBoxInputService*)CGameScreen::g_pInputService, gameTime);

	if(m_iSelectedEntry == -1)
		m_iSelectedEntry = 0;
}

void EndGameScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();
	CQuorumMissionManager* missionManager = (CQuorumMissionManager*)this->Get_Game()->Get_Services()->GetService("IMissionManager");

	m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);// | D3DX10_SPRITE_SORT_TEXTURE);
	
	m_pSpriteBatch->DrawBuffered(sprites[0].GetSprite(), 1);
	if(missionManager->Get_CurrentMission()->Get_HasWon())
	{
		m_pSpriteBatch->DrawBuffered(sprites[1].GetSprite(), 1);
	}
	else
	{
		m_pSpriteBatch->DrawBuffered(sprites[2].GetSprite(), 1);
		this->winMsg = "GAME OVER!!!";
	}

	m_pSpriteBatch->End();

	//Draw Text
	RECT rect = {(float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.2f, 20, 0, 0};
	char m_sBuffer[256];
	char m_sBuffer2[256];
	sprintf(m_sBuffer, this->winMsg);
	sprintf(m_sBuffer2, "Score: %.2f", this->m_dScore);

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height = 24;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(fontDesc.FaceName, "Ariel");

	this->m_pSpriteBatch->SetFont(fontDesc);
	m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);// | D3DX10_SPRITE_SORT_TEXTURE);

	this->m_pSpriteBatch->DrawString(m_sBuffer, &rect, D3DXCOLOR(1,1,1,1));

	rect.left = (float)this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.2f;
	rect.top = 40;

	this->m_pSpriteBatch->DrawString(m_sBuffer2, &rect, D3DXCOLOR(1,1,1,1));
	
	m_pSpriteBatch->End();

	RECT clientRect = this->Get_Game()->Get_Window()->Get_ClientRectangle();
	fontDesc.Height = clientRect.right / 80;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(fontDesc.FaceName, "Ariel");
	this->m_pSpriteBatch->SetFont(fontDesc);
}

void EndGameScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
	//m_bIsEnabled = true;

	m_iSelectedEntry = -1;
}

void EndGameScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
	//m_bIsEnabled = false;
}

void EndGameScreen::CalculateScore()
{
	if(this->m_dTimeToWin == -1.0f)// || this->m_iNumEnemiesKilled == -1)
		return;

	CQuorumSceneManager* sceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	this->m_iNumEnemiesKilled = sceneManager->Get_NumberOfKills();
	CQuorumMissionManager* missionManager = (CQuorumMissionManager*)this->Get_Game()->Get_Services()->GetService("IMissionManager");
	if(missionManager->Get_CurrentMission()->Get_HasWon())
	{
		this->m_dScore = (100000 / this->m_dTimeToWin) + (this->m_iNumEnemiesKilled * 100);
	}
	else
	{
		this->m_dScore = (this->m_iNumEnemiesKilled * 100);
	}

	//Select Message
	if(this->m_dTimeToWin > 1000)
	{
		if(this->m_iNumEnemiesKilled > 350)
		{
			//Cautious Enemy Annihilator
			winMsg = "Cautious Enemy Annihilator";
		}
		else if(this->m_iNumEnemiesKilled > 100)
		{
			//Cautious Average Killer
			winMsg = "Cautious Average Killer";
		}
		else
		{
			//Umm, what the hell were you doing?
			winMsg = "Umm, what the hell were you doing?";
		}
	}
	else if(this->m_dTimeToWin > 500)
	{
		if(this->m_iNumEnemiesKilled > 350)
		{
			//Good Enemy Annihilator
			winMsg = "Good Enemy Annihilator";
		}
		else if(this->m_iNumEnemiesKilled > 100)
		{
			//Average Enemy Killer
			winMsg = "Average Enemy Killer";
		}
		else
		{
			//Quick finisher, bad killer
			winMsg = "Quick finisher, bad killer";
		}
	}
	else if(this->m_dTimeToWin > 150)
	{
		if(this->m_iNumEnemiesKilled > 350)
		{
			//God
			winMsg = "God";
		}
		else if(this->m_iNumEnemiesKilled > 100)
		{
			//Demi-God
			winMsg = "Demi-God";
		}
		else
		{
			//Speedy but cowardly
			winMsg = "Speedy but cowardly";
		}
	}
	else
	{
		if(this->m_iNumEnemiesKilled > 350)
		{
			//God++
			winMsg = "God++";
		}
		else if(this->m_iNumEnemiesKilled > 100)
		{
			//Demi-God++
			winMsg = "Demi-God++";
		}
		else
		{
			//Time Warper but no guts
			winMsg = "Time Warper but no guts";
		}
	}

	this->m_bHasScored = true;
}

#pragma region Overridden Methods
char* EndGameScreen::Get_Type()
{
	return "EndGameScreen";
}
#pragma endregion