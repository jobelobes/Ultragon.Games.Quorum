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

enum GameObject : unsigned short int
{
	Bullet = 0,
	Enemy = 1,
	Explosion = 2,
	EnvironmentObject = 3,
};

enum GameObjectCommand : unsigned short int
{
	RequestCreate = 0,
	RequestDestroy = 1,
	Create = 2,
	Destroy = 3
};

class QUORUM_COMPONENTS_API CQuorumSceneManager : public CSceneManager
{ 
	private:
		#pragma region Nested Classes
		struct SCENE_PACKET
		{
			GameObject Object;
			GameObjectCommand Command;
			int Identifier;
		};

		struct BULLET_PACKET
		{
			GameObject Object;
			GameObjectCommand Command;
			int Identifier;
			BulletOwner Owner;
			BulletType Type;
			D3DXVECTOR2 Position;
			float Rotation;
			D3DXVECTOR2 Velocity;
		};

		struct ENEMY_PACKET
		{
			GameObject Object;
			GameObjectCommand Command;
			int Identifier;
			D3DXVECTOR2 Position;
			float Rotation;
		};
		#pragma endregion

		#pragma region Static Variables
		static INetworkService* g_pNetwork;
		#pragma endregion

		#pragma region Variables
		int m_iIdentifier;

		int m_iMaxBulletCount;
		CBullet** m_pBullets;

		int m_iMaxEnemyCount;
		CEnemyUnit** m_pEnemies;

		int m_iMaxAnimationCount;
		CAnimation** m_pAnimations;

		int m_iMaxEnvironmentObjectCount;
		CEnvironmentObject** m_pEnvObjects;

		int m_iMaxUnitControllers;
		CBasicUnitController** m_pUnitControllers;

		int m_iMaxCrewControllers;
		CBaseCrewController** m_pCrewControllers;

		CWorldBackground* m_pBackground;

		CShip* m_pShip;

		CGameContent* m_pGameContent;
		CQuorumMissionManager* m_pMissionManager;

		bool m_bRunningGame;
		int m_iNumEnemyKills;
		#pragma endregion

		#pragma region Properties
		bool Get_IsHost();
		#pragma endregion

		#pragma region Methods
		void InitializeBackground();

		void InitializeShip();

		void InitializeEnemies();
		CEnemyUnit* InternalCreateEnemy();

		void InitializeBullets();
		CBullet* InternalCreateBullet();

		void InitializeExplosions();
		CAnimation* InternalCreateExplosion();

		void InitializeWorld();
		CEnvironmentObject* InternalCreateEnvObject();

		void SendPacket(SCENE_PACKET);
		void SendPacket(BULLET_PACKET);
		void SendPacket(ENEMY_PACKET);

		void ProcessBullet(BULLET_PACKET*);
		void ProcessEnemy(ENEMY_PACKET*);
		#pragma endregion

	public:
		#pragma region Properties
		CShip* Get_Ship();
		void Set_Ship(CShip*);

		CGameContent* Get_GameContent();
		void Set_GameContent(CGameContent*);
		#pragma endregion

		#pragma region Constructors and Finalizers
		CQuorumSceneManager(CGame*);
		CQuorumSceneManager(CGame*, CGameContent*);
		~CQuorumSceneManager();
		#pragma endregion

		#pragma region Methods
		CBullet* CreateBullet(D3DXVECTOR2, D3DXVECTOR2, BulletOwner, BulletType, float);
		void DestroyBullet(CBullet*);

		CEnemyUnit* CreateEnemy(D3DXVECTOR2);
		void DestroyEnemy(CEnemyUnit*);

		CAnimation* CreateExplosion(D3DXVECTOR2);
		void DestroyExplosion();

		void CreateGame(bool, D3DX_SESSION_DESCRIPTION, int);
		void DestroyGame();

		CEnvironmentObject** CreateWorld();
		void DestroyWorld();
		void UpdateWorld();

		int Get_NumberOfKills();
		#pragma endregion

		#pragma region ISceneManager Methods
		virtual CGameComponent* Create(int);
		virtual void Destroy(int, CGameComponent*);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		#pragma endregion
};
