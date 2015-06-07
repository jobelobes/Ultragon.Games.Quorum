#include "Quorum.h"

#pragma region Methods
void CGame1::Initialize()
{
	masterVolume = 1.0f;

	srand(time(NULL));

	DWORD computerNameSize = MAX_COMPUTERNAME_LENGTH  + 1;
	char computerName[MAX_COMPUTERNAME_LENGTH + 1];
	GetComputerName(computerName,&computerNameSize);

	CGame::Initialize();

	this->Get_GraphicsDevice()->ToggleFullscreen();

	this->Get_Components()->Add(new CInputManager(this));
	this->Get_Components()->Add(new CQuorumCamera(this, 50));
	
	this->m_pSpriteBatch = new CSpriteBatch(this);
	this->m_pSceneManager = new CQuorumSceneManager(this, CGameContent::Load("..\\schema.xml"));

	this->Get_Components()->Add(this->m_pSceneManager);
	this->Get_Components()->Add(new CQuorumNetworkManager(this, "224.0.22.1", "9000", computerName));

	//Create Sound Engine
	this->Get_Components()->Add(new XACTEngine(this, "..\\Assets\\Audio\\Win\\musicfx.xgs"));	
	this->Get_Components()->Add(new CCollisionManager(this));

	CScreenManager* screenManager = new CScreenManager(this);	
	this->Get_Components()->Add(screenManager);

	screenManager->AddScreen(new TitleScreen(this));
	screenManager->AddScreen(new MainScreen(this));
	screenManager->AddScreen(new CreditsScreen(this));
	screenManager->AddScreen(new EndGameScreen(this));
	screenManager->AddScreen(new LobbyScreen(this));
	screenManager->AddScreen(new NewGameScreen(this));
	screenManager->AddScreen(new ControlsScreen(this));

	/*screenManager->AddScreen(new TestScreen(this));		
	screenManager->AddScreen(new OptionsScreen(this));
	screenManager->AddScreen(new PauseScreen(this));
	screenManager->AddScreen(new VideoOptionsScreen(this));
	screenManager->AddScreen(new VolumeOptionsScreen(this));*/
}

void CGame1::LoadContent()
{
	CGame::LoadContent();

	//Load Sounds
	IXACTEngineService* s = (IXACTEngineService*)this->Get_Services()->GetService("IXACTEngineService");
	s->LoadSoundBank("..\\Assets\\Audio\\Win\\soundBank.xsb");
	s->LoadWaveBank("..\\Assets\\Audio\\Win\\waveBank.xwb");
	
	IScreenManager* m_pScreenManager = (IScreenManager*)this->Get_Services()->GetService("IScreenManager");
	TitleScreen* sc = (TitleScreen*)m_pScreenManager->GetScreen("TitleScreen");
	sc->Activate(NULL);
}

void CGame1::Update(CGameTime* gameTime)
{
	CGame::Update(gameTime);


	CShip* ship = this->m_pSceneManager->Get_Ship();
	if(ship == NULL)
		sprintf(this->m_sBuffer, "%.2f FPS", 1.0 / gameTime->Get_ElapsedTime());
	else
		sprintf(this->m_sBuffer, "%.2f FPS\nShip World Coordinate: %.2f, %.2f", 1.0 / gameTime->Get_ElapsedTime(), ship->Get_Position().x, ship->Get_Position().y);

	INetworkService* networkService = (INetworkService*)this->Get_Services()->GetService("INetworkService");
}

void CGame1::Draw(CGameTime* gameTime)
{
	this->Get_GraphicsDevice()->Clear(D3DXCOLOR(0,0,0,0));

	CGame::Draw(gameTime);

	RECT rect = {5, 5, 0, 0};
	sprintf(this->m_sBuffer, "");
	this->m_pSpriteBatch->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT | D3DX10_SPRITE_SORT_TEXTURE);
	this->m_pSpriteBatch->DrawString(this->m_sBuffer, &rect, D3DXCOLOR(1,0,0,1));
	this->m_pSpriteBatch->End();
}
#pragma endregion