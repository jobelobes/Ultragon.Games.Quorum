#include "Quorum.h"

#pragma region Static Variables
INetworkService* NewGameScreen::g_pNetwork = NULL;
#pragma endregion

NewGameScreen::NewGameScreen(CGame* game)
: CGameScreen(game)
{
}

NewGameScreen::~NewGameScreen()
{

}

void NewGameScreen::Initialize()
{
	CGameComponent::Initialize();
}

void NewGameScreen::LoadContent()
{
	CDrawableGameComponent::LoadContent();

	RECT clientRect = this->Get_Game()->Get_Window()->Get_ClientRectangle();
	this->m_sClientsRect.left = (long)floor(clientRect.right * 0.71873);
	this->m_sClientsRect.top = (long)floor(clientRect.bottom * 0.739583);
	this->m_sClientsRect.right = 0;
	this->m_sClientsRect.bottom = 0;

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
	
	this->m_pSpriteBatch = (CSpriteBatch*)this->Get_Game()->Get_Services()->GetService("CSpriteBatch");
	this->m_pSpriteBatch->SetFont(fontDesc);

	if(NewGameScreen::g_pNetwork == NULL)
		NewGameScreen::g_pNetwork = (INetworkService*)this->Get_Game()->Get_Services()->GetService("INetworkService");

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

	CTexture2D* texture2 = texture2->Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\menu\\placeholders\\host_simple.png");//ArtAssets\\titleScreen_noname.png");
	CSprite2D sprite2;
	sprite2.SetTexture(texture2);
	D3DXMatrixTranslation(&matTranslation, this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Width / 2.0f,
		this->Get_Game()->Get_GraphicsDevice()->Get_Viewport()->Height / 2.0f, 0.0);
	sprite2.SetTransform(matScaling*matTranslation);
	sprites.push_back(sprite2);
	sprites.push_back(sprite);
}


void NewGameScreen::Update(CGameTime* gameTime)
{
	int offset = 0;
	char tmp[256];
	std::map<std::string, float> clients = NewGameScreen::g_pNetwork->Get_CurrentSession()->Get_Participants();
	std::map<std::string, float>::iterator clientIterator;
	for(clientIterator = clients.begin(); clientIterator != clients.end(); clientIterator++)
	{
		sprintf(tmp, "%s\n", (*clientIterator).first.c_str());
		memcpy(this->m_sClientBuffer + offset, tmp, sizeof(char) * strlen(tmp));
		offset += strlen(tmp);
	}

	sprintf(tmp, "\0");
	memcpy(this->m_sClientBuffer + offset, tmp, sizeof(char));

	CGameScreen::Update(gameTime);
}

void NewGameScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(DIK_RETURN))
	{
		if(NewGameScreen::g_pNetwork->Get_CurrentSession() != NULL)
		{
			

			if(NewGameScreen::g_pNetwork->Get_CurrentSession()->Get_IsHost())
			{	
				IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
				XACTINDEX upNoise;
				s->GetCueIndex("klaxon", &upNoise);
				s->PlaySound(upNoise);

				NewGameScreen::g_pNetwork->Get_CurrentSession()->StartGame();

				this->Deactivate();
			}
		}
	}	
	if(inputService->IsPressed(DIK_BACKSPACE))
	{
		IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
		XACTINDEX upNoise;
		s->GetCueIndex("explode_1", &upNoise);
		s->PlaySound(upNoise);

		if(NewGameScreen::g_pNetwork->Get_CurrentSession() != NULL)
		{
			if(NewGameScreen::g_pNetwork->Get_CurrentSession()->Get_IsHost())
				NewGameScreen::g_pNetwork->Get_CurrentSession()->DestroyGame();
		}

		this->m_pParentScreen->Activate(NULL);
		this->Deactivate();
	}	
}

void NewGameScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_A))
	{
		if(NewGameScreen::g_pNetwork->Get_CurrentSession() != NULL)
		{
			if(NewGameScreen::g_pNetwork->Get_CurrentSession()->Get_IsHost())
			{
				NewGameScreen::g_pNetwork->Get_CurrentSession()->StartGame();

				IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
				XACTINDEX upNoise;
				s->GetCueIndex("klaxon", &upNoise);
				s->PlaySound(upNoise);
			}
		}
	}	
	if(inputService->IsPressed(XButtonState::GAMEPAD_B))
	{
		if(NewGameScreen::g_pNetwork->Get_CurrentSession() != NULL)
		{
			IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
			XACTINDEX upNoise;
			s->GetCueIndex("explode_1", &upNoise);
			s->PlaySound(upNoise);

			if(NewGameScreen::g_pNetwork->Get_CurrentSession()->Get_IsHost())
				NewGameScreen::g_pNetwork->Get_CurrentSession()->DestroyGame();
		}

		this->m_pParentScreen->Activate(NULL);
		this->Deactivate();
	}	
}

void NewGameScreen::Draw(CGameTime* gameTime)
{
	int size = sprites.size();

	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
	{
		for(int i = 0; i < size; ++i)
			this->m_pSpriteBatch->DrawBuffered(sprites[i].GetSprite(), 1);
	}
	this->m_pSpriteBatch->End();

	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
	{
		this->m_pSpriteBatch->DrawString(this->m_sClientBuffer, &this->m_sClientsRect, D3DXCOLOR(1,1,1,1));
	}
	this->m_pSpriteBatch->End();
}

void NewGameScreen::Activate(CGameScreen* pScreen)
{
	this->m_pParentScreen = pScreen;
	this->Set_Enabled(true);
	this->Set_Visible(true);
}

void NewGameScreen::Deactivate()
{
	this->Set_Enabled(false);
	this->Set_Visible(false);
}

#pragma region Overridden Methods
char* NewGameScreen::Get_Type()
{
	return "NewGameScreen";
}
#pragma endregion