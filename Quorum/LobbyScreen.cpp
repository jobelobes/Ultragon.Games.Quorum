#include "Quorum.h"

#pragma region Static Variables
INetworkService* LobbyScreen::g_pNetwork = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
LobbyScreen::LobbyScreen(CGame* game)
: CGameScreen(game)
{	
	this->m_sClientsRect.left = 460;
	this->m_sClientsRect.top = 345;
	this->m_sClientsRect.right = 0;
	this->m_sClientsRect.bottom = 0;

	this->m_iSelectedGame = 0;
	this->m_iGameCount = 0;
	this->m_sGamesRect.left = 460;
	this->m_sGamesRect.top = 30;
	this->m_sGamesRect.right = 0;
	this->m_sGamesRect.bottom = 0;
}

LobbyScreen::~LobbyScreen()
{

}
#pragma endregion

#pragma region Overriden Methods
void LobbyScreen::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	RECT clientRect = this->Get_Game()->Get_Window()->Get_ClientRectangle();
	this->m_sClientsRect.left = (long)floor(clientRect.right * 0.71875);
	this->m_sClientsRect.top = (long)floor(clientRect.bottom * 0.71875);
	this->m_sClientsRect.right = 0;
	this->m_sClientsRect.bottom = 0;

	this->m_sGamesRect.left = (long)floor(clientRect.right * 0.71875);
	this->m_sGamesRect.top = (long)floor(clientRect.bottom * 0.0625);
	this->m_sGamesRect.right = 0;
	this->m_sGamesRect.bottom = 0;

	D3DX10_FONT_DESC fontDesc;
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
	
	CGraphicsDevice* device = this->Get_Game()->Get_GraphicsDevice();
	CServiceManager* serviceManager = this->Get_Game()->Get_Services();

	this->m_pSpriteBatch = (CSpriteBatch*)serviceManager->GetService("CSpriteBatch");
	this->m_pSpriteBatch->SetFont(fontDesc);

	if(LobbyScreen::g_pNetwork == NULL)
		LobbyScreen::g_pNetwork = (INetworkService*)serviceManager->GetService("INetworkService");

	CSprite2D sprite, sprite2;
	sprite.SetTexture(CTexture2D::Load(device, "..\\ArtAssets\\starbg.png"));
	sprite2.SetTexture(CTexture2D::Load(device, "..\\Assets\\Art\\menu\\placeholders\\lobby.png"));

	D3DXMATRIX matScaling;
	D3DXMATRIX matTranslation;

	D3DXMatrixScaling(&matScaling, device->Get_Viewport()->Width, device->Get_Viewport()->Height, 1.0f);
	D3DXMatrixTranslation(&matTranslation, device->Get_Viewport()->Width / 2.0f, device->Get_Viewport()->Height / 2.0f, 0.1);
	sprite.SetTransform(matScaling * matTranslation);
	
	D3DXMatrixTranslation(&matTranslation, device->Get_Viewport()->Width / 2.0f, device->Get_Viewport()->Height / 2.0f, 0.0);
	sprite2.SetTransform(matScaling * matTranslation);
	sprites.push_back(sprite2);
	sprites.push_back(sprite);
}

void LobbyScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(DIK_ESCAPE))
		exit(0);
	else if(inputService->IsPressed(DIK_BACK) && this->m_pParentScreen != NULL)
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		this->m_pParentScreen->Activate(NULL);
		this->Deactivate();
	}
	else if(inputService->IsPressed(DIK_DOWNARROW))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_iGameCount == 0)
			this->m_iSelectedGame = -1;
		else
			this->m_iSelectedGame = (this->m_iSelectedGame - 1) % this->m_iGameCount;
	}
	else if(inputService->IsPressed(DIK_UPARROW))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_iGameCount == 0)
			this->m_iSelectedGame = -1;
		else
			this->m_iSelectedGame = (this->m_iSelectedGame + 1) % this->m_iGameCount;
	}
	else if(inputService->IsPressed(DIK_RETURN) && this->m_iSelectedGame > -1)
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);

		std::map<unsigned int, D3DX_SESSION_DESCRIPTION> games = LobbyScreen::g_pNetwork->Find(NetworkSessionType::Local);
		this->JoinGame(games[this->m_iSelectedGameIdentifier]);
	}
	else if(inputService->IsPressed(DIK_INSERT))
	{
		this->CreateGame();

		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);
	}
}

void LobbyScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_BACK))
		exit(0);
	else if(inputService->IsPressed(XButtonState::GAMEPAD_B) && this->m_pParentScreen != NULL)
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		this->m_pParentScreen->Activate(NULL);
		this->Deactivate();
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_DOWN))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_down", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_iGameCount == 0)
			this->m_iSelectedGame = -1;
		else
			this->m_iSelectedGame = (this->m_iSelectedGame - 1) % this->m_iGameCount;
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_DPAD_UP))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("jump_up", &upNoise);
		s->PlaySound(upNoise);

		if(this->m_iGameCount == 0)
			this->m_iSelectedGame = -1;
		else
			this->m_iSelectedGame = (this->m_iSelectedGame + 1) % this->m_iGameCount;
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_A) && this->m_iSelectedGame > -1)
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);

		std::map<unsigned int, D3DX_SESSION_DESCRIPTION> games = LobbyScreen::g_pNetwork->Find(NetworkSessionType::Local);
		this->JoinGame(games[this->m_iSelectedGameIdentifier]);
	}
	else if(inputService->IsPressed(XButtonState::GAMEPAD_Y))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("turret_0", &upNoise);
		s->PlaySound(upNoise);

		this->CreateGame();
	}
}

void LobbyScreen::CreateGame()
{
	LobbyScreen::g_pNetwork->CreateSession(NetworkSessionType::Local, 4);

	NewGameScreen* sc = (NewGameScreen*)CGameScreen::g_pScreenManager->GetScreen("NewGameScreen");
	sc->Activate(this);
	this->Deactivate();
}

void LobbyScreen::JoinGame(D3DX_SESSION_DESCRIPTION gameSession)
{
	LobbyScreen::g_pNetwork->Join(gameSession);

	NewGameScreen* sc = (NewGameScreen*)CGameScreen::g_pScreenManager->GetScreen("NewGameScreen");
	sc->Activate(this);
	this->Deactivate();
}

void LobbyScreen::Update(CGameTime* gameTime)
{
	int offset = 0;
	char tmp[256];
	std::map<std::string, float> clients = LobbyScreen::g_pNetwork->NetworkedClients();
	std::map<std::string, float>::iterator clientIterator;
	for(clientIterator = clients.begin(); clientIterator != clients.end(); clientIterator++)
	{
		sprintf(tmp, "%s\n", (*clientIterator).first.c_str());
		memcpy(this->m_sClientBuffer + offset, tmp, sizeof(char) * strlen(tmp));
		offset += strlen(tmp);
	}

	sprintf(tmp, "\0");
	memcpy(this->m_sClientBuffer + offset, tmp, sizeof(char));

	offset = 0;
	this->m_iGameCount = 0;
	std::map<unsigned int, D3DX_SESSION_DESCRIPTION> games = LobbyScreen::g_pNetwork->Find(NetworkSessionType::Local);
	std::map<unsigned int, D3DX_SESSION_DESCRIPTION>::iterator gameIterator;
	for(gameIterator = games.begin(); gameIterator != games.end(); gameIterator++)
	{
		if(this->m_iSelectedGame == -1)
		{
			this->m_iSelectedGame = 0;
			this->m_iSelectedGameIdentifier = (*gameIterator).first;
		}
		else if(this->m_iGameCount == this->m_iSelectedGame)
			this->m_iSelectedGameIdentifier = (*gameIterator).first;

		sprintf(tmp, "%i [%i\\%i]\n", (*gameIterator).second.Identifier, (*gameIterator).second.OpenSlots, (*gameIterator).second.MaxPlayers);
		memcpy(this->m_sGamesBuffer + offset, tmp, sizeof(char) * strlen(tmp));
		offset += strlen(tmp);
		this->m_iGameCount++;
	}

	sprintf(tmp, "\0");
	memcpy(this->m_sGamesBuffer + offset, tmp, sizeof(char));

	if(this->m_iGameCount == 0)
		this->m_iSelectedGame = -1;

	CGameScreen::Update(gameTime);
}

void LobbyScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();

	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);

	for(int i = 0; i < size; ++i)
		m_pSpriteBatch->DrawBuffered(sprites[i].GetSprite(), 1);

	this->m_pSpriteBatch->End();

	if(this->m_iSelectedGame > -1)
	{
		CSquare::Draw(
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 
			D3DXVECTOR3(15, 0, -18 + this->m_iSelectedGame),
			D3DXVECTOR3(31, 0, -17 + this->m_iSelectedGame),
			this->Get_Game()->Get_GraphicsDevice(),
			CGameScreen::g_pCameraService);
	}

	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
	this->m_pSpriteBatch->DrawString(this->m_sClientBuffer, &this->m_sClientsRect, D3DXCOLOR(1,1,1,1));
	this->m_pSpriteBatch->DrawString(this->m_sGamesBuffer, &this->m_sGamesRect, D3DXCOLOR(1,1,1,1));
	this->m_pSpriteBatch->End();
}

void LobbyScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);

	this->m_iSelectedGame = 0;
}

void LobbyScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
}

char* LobbyScreen::Get_Type()
{
	return "LobbyScreen";
}
#pragma endregion