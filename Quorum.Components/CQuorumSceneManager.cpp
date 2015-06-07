/**
 * @description 
 *   Derives from the base CSceneManager and adds the ability for the user
 *   to create and destroy managed objects such as the bullets, enemies and
 *   explosions.  This allows the game to limit the amount of "new" calls that
 *   are made during the update-draw cycle.
 *
 * @version 1.0.0.0
 * @author	Jonathan Lobaugh
 *
 */

#include "Quorum.Components.h"

#pragma region Static Variables
INetworkService* CQuorumSceneManager::g_pNetwork = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CQuorumSceneManager::CQuorumSceneManager(CGame* game)
: CSceneManager(game)
{
	this->m_bRunningGame = false;
	this->m_pShip = NULL;
	this->m_pGameContent = NULL;
	this->m_pMissionManager = NULL;
	this->m_iIdentifier = CQuorumIdentifiers::Quorum_SceneManager;
	this->m_iNumEnemyKills = 0;
}

CQuorumSceneManager::CQuorumSceneManager(CGame* game, CGameContent* content)
: CSceneManager(game)
{
	this->m_bRunningGame = false;
	this->m_pShip = NULL;
	this->m_pGameContent = content;
	this->m_pMissionManager = NULL;
	this->m_iIdentifier = CQuorumIdentifiers::Quorum_SceneManager;
	this->m_iNumEnemyKills = 0;
}

CQuorumSceneManager::~CQuorumSceneManager()
{
	if(this->m_pBullets != NULL)
		delete [] this->m_pBullets;
}
#pragma endregion

#pragma region Properties
CShip* CQuorumSceneManager::Get_Ship()
{
	return this->m_pShip;
}

void CQuorumSceneManager::Set_Ship(CShip* ship)
{
	this->m_pShip = ship;
}

CGameContent* CQuorumSceneManager::Get_GameContent()
{
	return this->m_pGameContent;
}

void CQuorumSceneManager::Set_GameContent(CGameContent* value)
{
	this->m_pGameContent = value;
}

bool CQuorumSceneManager::Get_IsHost()
{
	if(CQuorumSceneManager::g_pNetwork != NULL && CQuorumSceneManager::g_pNetwork->Get_CurrentSession() != NULL)
		return CQuorumSceneManager::g_pNetwork->Get_CurrentSession()->Get_IsHost();
	return true;
}
#pragma endregion

#pragma region Methods
void CQuorumSceneManager::InitializeBackground()
{
	CGame* game = this->Get_Game();

	//Load Texture Assets
	CTexture2D* text;
	std::vector<ASSET*> assets;
	this->m_pGameContent->Get_Assets(assets);
	for(int i = 0; i < assets.size(); ++i)
	{
		char* path = this->m_pGameContent->Get_GlobalPath();
		strcat(path, assets[i]->path);
		strcat(path, assets[i]->fileName);
		text = CTexture2D::Load(game->Get_GraphicsDevice(), path);
	}

	std::vector<LAYER*> layers;
	this->m_pGameContent->Get_Layers(layers);

	int size = layers.size();
	CParallaxBackground* layer = NULL;
	this->m_pBackground = new CWorldBackground(game);
	for(int i = 0; i < size + 1; ++i)
	{
		if(i > 0)
		{
			layer = new CParallaxBackground(game,
				D3DXVECTOR2(layers[i-1]->mapSize.x,layers[i-1]->mapSize.y),
				D3DXVECTOR2(layers[i-1]->tileSize.x,layers[i-1]->tileSize.y),
				layers[i-1]->parallaxSpeed);
			layer->Set_Texture(text);
		}
		else
		{
			layer = new CParallaxBackground(game, D3DXVECTOR2(3,3), D3DXVECTOR2(120, 60), 1);
			layer->Set_Texture(CTexture2D::Load(game->Get_GraphicsDevice(), "..\\Assets\\Art\\background_0.png"));
		}

		if(layer != NULL)
			this->m_pBackground->AddLayer(layer);
	}

	this->m_pBackground->Set_Enabled(false);
	this->m_pBackground->Set_Visible(false);
	game->Get_Components()->Add(this->m_pBackground);
}

void CQuorumSceneManager::InitializeShip()
{
	this->m_pShip = new CShip(this->Get_Game(), CQuorumIdentifiers::Quorum_Ship);
	this->m_pShip->Set_Enabled(false);
	this->m_pShip->Set_Visible(false);
	this->Get_Game()->Get_Components()->Add(this->m_pShip);
}

void CQuorumSceneManager::InitializeBullets()
{
	this->m_iMaxBulletCount = 200;
	this->m_pBullets = new CBullet*[this->m_iMaxBulletCount];
	
	for(int i = 0; i < this->m_iMaxBulletCount; i++)
	{
		this->m_pBullets[i] = new CBullet(this->Get_Game(), CQuorumIdentifiers::Quorum_Bullet + i);
		this->m_pBullets[i]->Set_Enabled(false);
		this->m_pBullets[i]->Set_Visible(false);
		this->Get_Game()->Get_Components()->Add(this->m_pBullets[i]);
	}
}

void CQuorumSceneManager::InitializeWorld()
{
	std::vector<ENVIRONMENTOBJECT*> envObjects;
	this->m_pGameContent->Get_EnvironmentObjects(envObjects);

	this->m_iMaxEnvironmentObjectCount = envObjects.size();
	this->m_pEnvObjects = new CEnvironmentObject*[this->m_iMaxEnvironmentObjectCount];
	for(int i = 0; i < this->m_iMaxEnvironmentObjectCount; i++)
	{
		this->m_pEnvObjects[i] = new CEnvironmentObject(this->Get_Game(), (EnvironmentObjectType)envObjects[i]->type);
		this->m_pEnvObjects[i]->Set_Enabled(false);
		this->m_pEnvObjects[i]->Set_Visible(false);	

		this->Get_Game()->Get_Components()->Add(this->m_pEnvObjects[i]);
	}
}

CBullet* CQuorumSceneManager::InternalCreateBullet()
{
	for(int i = 0; i < this->m_iMaxBulletCount; i++)
	{
		if(this->m_pBullets[i]->Get_Enabled())
			continue;

		this->m_pBullets[i]->Set_Enabled(true);
		this->m_pBullets[i]->Set_Visible(true);

		return this->m_pBullets[i];
	}

	return NULL;
}

CBullet* CQuorumSceneManager::CreateBullet(D3DXVECTOR2 position, D3DXVECTOR2 velocity, BulletOwner owner, BulletType type, float lifespan)
{
	if(this->Get_IsHost())
	{
		CBullet* bullet = this->InternalCreateBullet();
		if(bullet != NULL)
		{
			bullet->Set_Owner(owner);
			bullet->Set_Position(position);
			bullet->Set_Velocity(velocity);
			bullet->Set_BulletType(type);
			bullet->Get_Animation()->Start(true);
			bullet->Set_Lifespan(lifespan);

			BULLET_PACKET packet;
			packet.Object = GameObject::Bullet;
			packet.Command = GameObjectCommand::Create;
			packet.Owner = owner;
			packet.Type = type;
			packet.Identifier = bullet->Get_Identifier();
			packet.Position = position;
			packet.Velocity = velocity;

			this->SendPacket(packet);
		}
		return bullet;
	}
	else
	{
		BULLET_PACKET packet;
		packet.Object = GameObject::Bullet;
		packet.Command = GameObjectCommand::RequestCreate;
		packet.Owner = owner;
		packet.Type = type;
		packet.Position = position;
		packet.Velocity = velocity;

		this->SendPacket(packet);
	}

	return NULL;	
}

void CQuorumSceneManager::DestroyBullet(CBullet* bullet)
{
	if(this->Get_IsHost())
	{
		bullet->Set_Enabled(false);
		bullet->Set_Visible(false);
		bullet->Get_Animation()->Stop();

		SCENE_PACKET packet;
		packet.Object = GameObject::Bullet;
		packet.Command = GameObjectCommand::Destroy;
		packet.Identifier = bullet->Get_Identifier();
		this->SendPacket(packet);
	}
}

void CQuorumSceneManager::InitializeEnemies()
{
	this->m_iMaxEnemyCount = 100;
	this->m_pEnemies = new CEnemyUnit*[this->m_iMaxEnemyCount];

	for(int i = 0; i < this->m_iMaxEnemyCount; i++)
	{
		this->m_pEnemies[i] = new CEnemyUnit(this->Get_Game(), CQuorumIdentifiers::Quorum_Enemy + i);
		this->m_pEnemies[i]->Set_Enabled(false);
		this->m_pEnemies[i]->Set_Visible(false);
		this->Get_Game()->Get_Components()->Add(this->m_pEnemies[i]);
	}
}

CEnemyUnit* CQuorumSceneManager::InternalCreateEnemy()
{
	for(int i = 0; i < this->m_iMaxEnemyCount; i++)
	{
		if(this->m_pEnemies[i]->Get_Enabled())
			continue;

		this->m_pEnemies[i]->Set_Enabled(true);
		this->m_pEnemies[i]->Set_Visible(true);

		if(this->m_pShip != NULL)
			this->m_pEnemies[i]->Set_Target(this->m_pShip);

		return this->m_pEnemies[i];
	}

	return NULL;
}


CEnemyUnit* CQuorumSceneManager::CreateEnemy(D3DXVECTOR2 position)
{
	if(this->Get_IsHost())
	{
		CEnemyUnit* enemy = this->InternalCreateEnemy();
		if(enemy != NULL)
		{
			enemy->Set_Position(position);

			ENEMY_PACKET packet;
			packet.Object = GameObject::Enemy;
			packet.Command = GameObjectCommand::Create;
			packet.Identifier = enemy->Get_Identifier();
			packet.Position = position;
			this->SendPacket(packet);
		}

		return enemy;
	}
	return NULL;
}

void CQuorumSceneManager::DestroyEnemy(CEnemyUnit* enemy)
{

	if(this->Get_IsHost())
	{
		enemy->Set_Enabled(false);
		enemy->Set_Visible(false);

		ENEMY_PACKET packet;
		packet.Object = GameObject::Enemy;
		packet.Command = GameObjectCommand::Destroy;
		packet.Identifier = enemy->Get_Identifier();
		this->SendPacket(packet);
	}
}

void CQuorumSceneManager::UpdateWorld()
{
	std::vector<ENVIRONMENTOBJECT*> envObjects;

	this->m_pGameContent->Get_EnvironmentObjects(envObjects);

	double x = 0;
	double y = 0;

	for(int i = 0; i < this->m_iMaxEnvironmentObjectCount; i++)
	{
		x = abs(this->m_pEnvObjects[i]->Get_Position().x - this->m_pShip->Get_Position().x);
		y = abs(this->m_pEnvObjects[i]->Get_Position().y - this->m_pShip->Get_Position().y);

		if(x <= 100 && y <= 100)
		{
			this->m_pEnvObjects[i]->Set_Position(D3DXVECTOR2(envObjects[i]->positionX, envObjects[i]->positionY));
			this->m_pEnvObjects[i]->Set_Scale(D3DXVECTOR2(envObjects[i]->scaleX, envObjects[i]->scaleY));
			this->m_pEnvObjects[i]->Set_Enabled(true);
			this->m_pEnvObjects[i]->Set_Visible(true);
			if(this->m_pEnvObjects[i]->Get_Animation() != NULL && !this->m_pEnvObjects[i]->Get_Animation()->Get_IsRunning())
				this->m_pEnvObjects[i]->Get_Animation()->Start(true);
		}else{
			this->m_pEnvObjects[i]->Set_Enabled(false);
			this->m_pEnvObjects[i]->Set_Visible(false);
			if(this->m_pEnvObjects[i]->Get_Animation() != NULL)
				this->m_pEnvObjects[i]->Get_Animation()->Stop();
		}
	}
}

void CQuorumSceneManager::DestroyWorld()
{
	for(int i = 0; i < this->m_iMaxEnvironmentObjectCount; i++)
	{
		this->m_pEnvObjects[i]->Set_Enabled(false);
		this->m_pEnvObjects[i]->Set_Visible(false);
	}
}
void CQuorumSceneManager::InitializeExplosions()
{
	CTexture2D* texture = CTexture2D::Load(this->Get_Game()->Get_GraphicsDevice(), "..\\Assets\\Art\\explosion.png");
	this->m_iMaxAnimationCount = 100;
	this->m_pAnimations = new CAnimation*[this->m_iMaxAnimationCount];
	for(int i = 0; i < this->m_iMaxAnimationCount; i++)
	{
		this->m_pAnimations[i] = new CAnimation(this->Get_Game());
		this->m_pAnimations[i]->Set_ImageSize(D3DXVECTOR2(64, 64));
		this->m_pAnimations[i]->Set_TextureSize(D3DXVECTOR2(512, 512));
		this->m_pAnimations[i]->Set_ImageScale(D3DXVECTOR2(7,7));
		this->m_pAnimations[i]->Set_FramePerSecond(60);
		this->m_pAnimations[i]->Set_NumberOfFrames(64);
		this->m_pAnimations[i]->Set_Texture(texture);
		this->m_pAnimations[i]->Set_IsLooping(false);
		this->m_pAnimations[i]->Set_Enabled(false);
		this->m_pAnimations[i]->Set_Visible(false);
		this->Get_Game()->Get_Components()->Add(this->m_pAnimations[i]);
	}
}

CAnimation* CQuorumSceneManager::InternalCreateExplosion()
{
	// Play an explosion sound:
	IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
	XACTINDEX explosion;
	s->GetCueIndex("explode_1", &explosion);
	s->PlaySound(explosion);

	for(int i = 0; i < this->m_iMaxAnimationCount; i++)
	{
		if(!this->m_pAnimations[i]->Get_Enabled())
		{
			this->m_pAnimations[i]->Set_Enabled(true);
			this->m_pAnimations[i]->Set_Visible(true);
			return this->m_pAnimations[i];
		}
	}

	return NULL;
}

CEnvironmentObject* CQuorumSceneManager::InternalCreateEnvObject()
{
	for(int i = 0; i < this->m_iMaxEnvironmentObjectCount; i++)
	{
		if(!this->m_pEnvObjects[i]->Get_Enabled())
		{
			this->m_pEnvObjects[i]->Set_Enabled(true);
			this->m_pEnvObjects[i]->Set_Visible(true);
			return this->m_pEnvObjects[i];
		}
	}

	return NULL;
}
CAnimation* CQuorumSceneManager::CreateExplosion(D3DXVECTOR2 position)
{

	CAnimation* animation = this->InternalCreateExplosion();
	if(animation != NULL)
		animation->Set_Position(position);

	return animation;
}

void CQuorumSceneManager::CreateGame(bool host, D3DX_SESSION_DESCRIPTION session, int playerIndex)
{
	CGame* game = this->Get_Game();

	this->m_iMaxUnitControllers = 100;
	this->m_pUnitControllers = new CBasicUnitController*[this->m_iMaxUnitControllers];


	// Kill the dumb music:
	IXACTEngineService* s = (IXACTEngineService*)this->Get_Game()->Get_Services()->GetService("IXACTEngineService");
	XACTINDEX introLoop;
	// silentHack is an empty wave file that causes the previous sound to fade out. Yes, I'm lazy.
	s->GetCueIndex("silentHack", &introLoop);
	s->PlaySound(introLoop);

	// Start the real music:
	
	// intro is the startup loop for real music, will play once
	s->GetCueIndex("intro", &introLoop);
	s->PlaySound(introLoop);

	// loop1 is the song that never ends. Deal with it.
	s->GetCueIndex("loop1", &introLoop);
	s->PlaySound(introLoop);


	// create appropriate enemy controllers
	//CBaseEnemyController* enemyController = NULL;
	for(int i = 0; i < this->m_iMaxEnemyCount; i++)
	{
		if(host)
		{
			int m = rand() % 3;
			AIMode mode;
			switch(m)
			{
			case 0:
				mode = AIMode::Mode0;
				break;
			case 1:
				mode = AIMode::Mode1;
				break;
			case 2:
				mode = AIMode::Mode2;
				break;
			}
			this->m_pUnitControllers[i] = new CAIEnemyController(game, mode);
		}
		else
			this->m_pUnitControllers[i] = new CNetworkEnemyController(game);

		this->m_pUnitControllers[i]->Bind(this->m_pEnemies[i]);
		game->Get_Components()->Add(this->m_pUnitControllers[i]);
	}

	// build crew members + controllers
	this->m_iMaxCrewControllers = 4;
	this->m_pCrewControllers = new CBaseCrewController*[this->m_iMaxCrewControllers];
	for(int i = 0; i < (session.MaxPlayers - session.OpenSlots); i++)
	{
		CCrew* crew = new CCrew(game, this->m_pShip, CQuorumIdentifiers::Quorum_Crew + i);
		game->Get_Components()->Add(crew);

		if((i + 1) == playerIndex)
			this->m_pCrewControllers[i] = new CPlayerCrewController(game);
		else
			this->m_pCrewControllers[i] = new CNetworkCrewController(game);

		this->m_pCrewControllers[i]->Bind(crew);
		game->Get_Components()->Add(this->m_pCrewControllers[i]);
	}

	// set mission
	this->m_pMissionManager->Set_CurrentMission(0);

	this->m_pBackground->Set_Enabled(true);
	this->m_pBackground->Set_Visible(true);

	this->m_pShip->Set_Enabled(true);
	this->m_pShip->Set_Visible(true);

	this->m_bRunningGame = true;

	// start the sound for the ship
	// Every time we get the move rate, update the sound pitch?
	XACTINDEX engineIndex = 4;
	//s->GetWaveIndex("engine", &engineIndex); // why no worky!?

	// hard coded at 4 and I don't care!
	s->PlayWave(engineIndex, XACT_FLAG_UNITS_MS, 0, XACTLOOPCOUNT_INFINITE, &CShip::engineNoise);
	CShip::engineNoise->SetPitch(XACTPITCH_MIN);
	CShip::engineNoise->SetVolume(0);

}

void CQuorumSceneManager::DestroyGame()
{
	if(!this->m_bRunningGame)
		return;

	this->m_bRunningGame = false;

	for(int i = 0; i < this->m_iMaxEnemyCount; ++i)
	{
		this->m_pEnemies[i]->Set_Enabled(false);
		this->m_pEnemies[i]->Set_Visible(false);
	}
	for(int i = 0; i < this->m_iMaxBulletCount; ++i)
	{
		this->m_pBullets[i]->Set_Enabled(false);
		this->m_pBullets[i]->Set_Visible(false);
	}	

	this->m_pShip->Set_Enabled(false);
	this->m_pShip->Set_Visible(false);

	this->m_pBackground->Set_Enabled(false);
	this->m_pBackground->Set_Visible(false);

	DestroyWorld();

	//unbind controllers
	for(int i = 0; i < this->m_iMaxUnitControllers; ++i)
		this->m_pUnitControllers[i]->Unbind();

	/*for(int i = 0; i < this->m_iMaxCrewControllers; ++i)
		this->m_pCrewControllers[i]->Unbind();*/
}

void CQuorumSceneManager::SendPacket(SCENE_PACKET scenePacket)
{
	if(CQuorumSceneManager::g_pNetwork != NULL && CQuorumSceneManager::g_pNetwork->Get_CurrentSession() != NULL)
	{
		Packet packet;
		packet.Command = PacketCommand::SendData;
		packet.Identifier = this->m_iIdentifier;
		packet.SetData((char*)&scenePacket, 0, sizeof(SCENE_PACKET));
		CQuorumSceneManager::g_pNetwork->Get_CurrentSession()->SendPacket(packet);
	}	
}

void CQuorumSceneManager::SendPacket(BULLET_PACKET bulletPacket)
{
	if(CQuorumSceneManager::g_pNetwork != NULL && CQuorumSceneManager::g_pNetwork->Get_CurrentSession() != NULL)
	{
		Packet packet;
		packet.Command = PacketCommand::SendData;
		packet.Identifier = this->m_iIdentifier;
		packet.SetData((char*)&bulletPacket, 0, sizeof(BULLET_PACKET));
		CQuorumSceneManager::g_pNetwork->Get_CurrentSession()->SendPacket(packet);
	}	
}

void CQuorumSceneManager::SendPacket(ENEMY_PACKET enemyPacket)
{
	if(CQuorumSceneManager::g_pNetwork != NULL && CQuorumSceneManager::g_pNetwork->Get_CurrentSession() != NULL)
	{
		Packet packet;
		packet.Command = PacketCommand::SendData;
		packet.Identifier = this->m_iIdentifier;
		packet.SetData((char*)&enemyPacket, 0, sizeof(ENEMY_PACKET));
		CQuorumSceneManager::g_pNetwork->Get_CurrentSession()->SendPacket(packet);
	}	
}

void CQuorumSceneManager::ProcessBullet(BULLET_PACKET* packet)
{
	CNetworkSession* session = CQuorumSceneManager::g_pNetwork->Get_CurrentSession();

	switch(packet->Command)
	{	
		case GameObjectCommand::Create:
		{
			if(!session->Get_IsHost())
			{
				printf("Host Create Bullet\n");

				int index = packet->Identifier - CQuorumIdentifiers::Quorum_Bullet;
				CBullet* bullet = this->m_pBullets[index];
				bullet->Set_Enabled(true);
				bullet->Set_Visible(true);
				bullet->Set_Owner(packet->Owner);
				bullet->Set_BulletType(packet->Type);
				bullet->Set_Position(packet->Position);
				bullet->Set_Velocity(packet->Velocity);
				bullet->Get_Animation()->Start(true);
			}
			break;
		}
		case GameObjectCommand::RequestCreate:
		{
			if(session->Get_IsHost())
			{
				printf("Client Request Create Bullet\n");
				this->CreateBullet(packet->Position, packet->Velocity, packet->Owner, packet->Type, CTurret::Get_StaticLifespan(packet->Type));
			}
			break;
		}
		case GameObjectCommand::Destroy:
		{
			if(!session->Get_IsHost())
			{
				printf("Host Destroy Bullet\n");
				
				int index = packet->Identifier - CQuorumIdentifiers::Quorum_Bullet;
				CBullet* bullet = this->m_pBullets[index];
				bullet->Set_Enabled(false);
				bullet->Set_Visible(false);
			}
			break;
		}
	}
}

void CQuorumSceneManager::ProcessEnemy(ENEMY_PACKET* packet)
{
	CNetworkSession* session = CQuorumSceneManager::g_pNetwork->Get_CurrentSession();
}

int CQuorumSceneManager::Get_NumberOfKills()
{
	return this->m_iNumEnemyKills;
}
#pragma endregion

#pragma region ISceneManager Methods
CGameComponent* CQuorumSceneManager::Create(int objectId)
{
	switch(objectId)
	{
		case GameObject::Bullet:
			return this->InternalCreateBullet();
			break;
		case GameObject::Enemy:
			return this->InternalCreateEnemy();
			break;
		case GameObject::Explosion:
			return this->InternalCreateExplosion();
			break;
		case GameObject::EnvironmentObject:
			return this->InternalCreateEnvObject();
		default:
			break;
	}
}

void CQuorumSceneManager::Destroy(int objectId, CGameComponent* object)
{
	switch(objectId)
	{
		case GameObject::Bullet:
			if(dynamic_cast<CBullet*>(object) != NULL)
				this->DestroyBullet((CBullet*)object);
			break;
		case GameObject::Enemy:
			if(dynamic_cast<CEnemyUnit*>(object) != NULL)
			{
				this->DestroyEnemy((CEnemyUnit*)object);
				this->m_iNumEnemyKills += 1;
			}
			break;
		case GameObject::EnvironmentObject:
			if(dynamic_cast<CEnvironmentObject*>(object) != NULL)
				this->DestroyWorld();
			break;
		default:
			break;
	}
}
#pragma endregion

#pragma region Overriden Methods
void CQuorumSceneManager::Initialize()
{
	CGameComponent::Initialize();

	if(CQuorumSceneManager::g_pNetwork == NULL)
		CQuorumSceneManager::g_pNetwork = (INetworkService*)this->Get_Game()->Get_Services()->GetService("INetworkService");

	if(this->m_pMissionManager == NULL)
	{
		this->m_pMissionManager = new CQuorumMissionManager(this->Get_Game(), this->m_pGameContent);
		this->Get_Game()->Get_Components()->Add(this->m_pMissionManager);
	}

	this->InitializeBackground();
	this->InitializeBullets();
	this->InitializeWorld();
	this->InitializeEnemies();
	this->InitializeShip();
	this->InitializeExplosions();
	
}

void CQuorumSceneManager::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	if(CQuorumSceneManager::g_pNetwork == NULL)
		return;

	CNetworkSession* session = CQuorumSceneManager::g_pNetwork->Get_CurrentSession();
	if(session != NULL)
	{
		Packet packet;
		while((packet = session->GetNextPacket(this->m_iIdentifier)).Command != PacketCommand::Empty)
		{
			if(packet.Command != PacketCommand::SendData)
				continue;

			SCENE_PACKET* scenePacket = (SCENE_PACKET*)packet.Data;
			switch(scenePacket->Object)
			{
				case GameObject::Bullet:
				{
					this->ProcessBullet((BULLET_PACKET*)packet.Data);
					break;
				}
				case GameObject::Enemy:
				{
					this->ProcessEnemy((ENEMY_PACKET*)packet.Data);
					break;
				}
			}
		}		
	}

	if(this->m_bRunningGame)
	{
		if(this->m_pMissionManager->Get_CurrentMission()->Check_GoalState())
			this->DestroyGame();
		else
			this->UpdateWorld();
	}
}
#pragma endregion