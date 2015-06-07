/**
 * @description 
 *   Defines the Main screen for the game. Screen that is used to draw all gameplay content.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.h"

#pragma region Constructors and Finalizers
MainScreen::MainScreen(CGame* game)
: CGameScreen(game)
{
	this->m_bActivated = false;
	this->m_pBackground = NULL;

	this->m_pArrowGuide = NULL;

	this->startTime = NULL;
}

MainScreen::~MainScreen()
{

}
#pragma endregion

#pragma region Overriden Methods
void MainScreen::Initialize()
{
	CGameComponent::Initialize();

	if(this->m_bActivated)
		this->Activate(NULL);
	else
		this->Deactivate();
}

void MainScreen::LoadContent()
{
	//Initialize Guide Arrow
	CTexture2D* arrowTexture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\arrow.png");//planet_1.png");
	this->m_pArrowGuide = new CAnimation(this->Get_Game());
	this->m_pArrowGuide->Set_ImageSize(D3DXVECTOR2(128, 128));
	this->m_pArrowGuide->Set_TextureSize(D3DXVECTOR2(128, 128));
	this->m_pArrowGuide->Set_ImageScale(D3DXVECTOR2(2,2));
	this->m_pArrowGuide->Set_FramePerSecond(60);
	this->m_pArrowGuide->Set_NumberOfFrames(1);
	this->m_pArrowGuide->Set_Texture(arrowTexture);
	this->m_pArrowGuide->Set_InitialFrame(0);
	this->m_pArrowGuide->Set_IsLooping(false);
	this->m_pArrowGuide->Set_Enabled(true);
	this->m_pArrowGuide->Set_Visible(false);
	this->m_pArrowGuide->Set_WaitOnFirstFrame(true);
	this->m_pArrowGuide->Set_Position(D3DXVECTOR2(0,0));
	this->m_pArrowGuide->Start();
	this->m_pArrowGuide->Pause(true);
	this->Get_Game()->Get_Components()->Add(this->m_pArrowGuide);
}

void MainScreen::Update(CGameTime* gameTime)
{
	if(startTime == NULL)
	{
		time(&startTime);
	}

	CGameScreen::Update(gameTime);

	ShowGuideArrow();

	CQuorumMissionManager* missionManager = (CQuorumMissionManager*)this->Get_Game()->Get_Services()->GetService("IMissionManager");
	if(missionManager->Get_IsGoalReached())
	{
		this->m_pArrowGuide->Set_Visible(false);
		this->m_pArrowGuide->Set_Enabled(false);

		time_t endTime;
		time(&endTime);
		double dif = difftime(endTime, startTime);

		CQuorumSceneManager* sceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
		sceneManager->DestroyWorld();
		sceneManager->DestroyGame();
		EndGameScreen* sc = (EndGameScreen*)CGameScreen::g_pScreenManager->GetScreen("EndGameScreen");
		sc->Activate(NULL);
		sc->SetTimeToWin(dif);
		Deactivate();
	}
}

char* MainScreen::Get_Type()
{
	return "MainScreen";
}
#pragma endregion

#pragma region Methods
void MainScreen::HandleWindowsInput(IWindowsInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(DIK_TAB))
		this->Get_Game()->Get_GraphicsDevice()->ToggleFullscreen();

	if(inputService->IsPressed(DIK_ESCAPE))
		this->Get_Game()->Exit();
}

void MainScreen::HandleXboxInput(IXBoxInputService* inputService, CGameTime* gameTime)
{
	if(inputService->IsPressed(XButtonState::GAMEPAD_BACK))
		this->Get_Game()->Exit();

	if(inputService->IsPressed(XButtonState::GAMEPAD_START))
		this->Get_Game()->Get_GraphicsDevice()->ToggleFullscreen();
}

void MainScreen::Activate(CGameScreen* pScreen)
{
	CGame* game = this->Get_Game();

	this->m_pParentScreen = pScreen;
	this->m_bActivated = true;

	/*this->Set_Enabled(true);
	this->Set_Visible(true);*/
}

void MainScreen::Deactivate()
{
	this->m_bActivated = false;

	/*this->Set_Enabled(false);
	this->Set_Visible(false);*/
}

void MainScreen::ShowGuideArrow()
{
	CQuorumMissionManager* missionManager = (CQuorumMissionManager*)this->Get_Game()->Get_Services()->GetService("IMissionManager");
	CQuorumSceneManager* sceneManager = (CQuorumSceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");
	CMission* mission = missionManager->Get_CurrentMission();
	
	if(mission == NULL)
		return;

	D3DXVECTOR2 pos = this->m_pArrowGuide->Get_Position();//D3DXVECTOR2(0,0);
	D3DXVECTOR2 goal = mission->Get_Goal();
	D3DXVECTOR2 diff = goal - sceneManager->Get_Ship()->Get_Position();
	float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
	float scale = 19;
	if(abs(diff.x) > 27 ||
		abs(diff.y) > 20)
	{
		this->m_pArrowGuide->Set_Visible(true);
		this->m_pArrowGuide->Set_Enabled(true);

		//Calculate translation
		/*if((diff.x / 27.0f) > (diff.y / 20.0f + this->m_pArrowGuide->Get_ImageScale().y))
		{
			scale = 27;
		}
		else
		{
			scale = 20;
		}*/
		pos = diff;
		D3DXVec2Normalize(&pos, &pos);
		pos *= scale;
		/*if((diff.x / 27.0f) > (diff.y / 20.0f + this->m_pArrowGuide->Get_ImageScale().y))
		{
			float dif = (scale - pos.x);
			pos.x = pos.x + dif;
			pos.y = pos.y + dif;
		}
		else
		{
			float dif = (scale - pos.y);
			pos.x = pos.x + dif;
			pos.y = pos.y + dif;
		}*/

		//Calculate rotation
		D3DXVECTOR2 origin = D3DXVECTOR2(1.0f, 0.0f);
		D3DXVec2Normalize(&diff, &diff);
		float cAng = D3DXVec2Dot(&origin, &diff);
		float ang = acos(cAng);
		float z = D3DXVec2CCW(&diff, &origin);
		if (z < 0) 
		{
			ang = (2.0f * D3DX_PI) - ang;
		}
		//ang += (D3DX_PI / 2.0f);

		//Create world matrix
		D3DXMATRIX* world = this->m_pArrowGuide->Get_WorldMatrix();
		D3DXMATRIX trans;
		D3DXMATRIX rot;
		D3DXMATRIX wTrans;
		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&rot);
		D3DXMatrixIdentity(&wTrans);//wTrans._41 = world->_41;wTrans._42 = world->_42;wTrans._43 = world->_43; 
		D3DXMatrixRotationY(&rot, ang);
		D3DXMatrixTranslation(&trans, pos.x, 0, pos.y);

		D3DXMatrixMultiply(world, &wTrans, &rot);
		D3DXMatrixMultiply(world, world, &trans);
		/*D3DXMatrixMultiply(&(this->m_pArrowTrans), &wTrans, &rot);
		D3DXMatrixMultiply(&(this->m_pArrowTrans), &wTrans, &trans);*/
		this->m_pArrowGuide->Set_WorldMatrix(world);//(this->m_pArrowTrans));
		
		//this->Get_BoundObject()->Rotate(ang + (D3DX_PI / 2.0f));
	}
	else
	{
		this->m_pArrowGuide->Set_Visible(false);
		this->m_pArrowGuide->Set_Enabled(false);
	}
}
#pragma endregion