#include "Quorum.Components.h"

#pragma region Static Variables
int CMission::g_iReferences = 0;
ISceneManager* CMission::g_pSceneManager;
#pragma endregin

#pragma region Constructors and Deconstructors
//Default constructor
CMission::CMission(CGame* game)
: CGameComponent(game)
{
	this->m_pMissionType;

	this->m_pStart = D3DXVECTOR2(0, 0);
	this->m_pGoal = D3DXVECTOR2(0, 0);

	this->m_iSpawnPointNum = 0;
	this->m_pEnemyGroups = new EnemyGroup*[1];

	this->m_sMissionName = "";
	this->m_iMissionID = 0;

	this->m_bWon = false;
	this->m_bSpawn = true;
	this->m_fSpawnTimer = 0.0f;
}

//Parameterized constructor
CMission::CMission(CGame* game, D3DXVECTOR2 startPoint, D3DXVECTOR2 goalPoint, int id, char* name, MissionType type, int enemyGroupNum)
:CGameComponent(game)
{
	CMission::g_iReferences++;

	this->m_pStart = startPoint;
	this->m_pGoal = goalPoint;
	this->m_iMissionID = id;
	this->m_sMissionName = name;
	this->m_pMissionType = type;

	this->m_pEnemyGroups = new EnemyGroup*[enemyGroupNum];
	this->m_iSpawnPointNum = enemyGroupNum;

	this->m_bSpawn = true;
	this->m_fSpawnTimer = 0.0f;
}

CMission::~CMission() 
{ 
	CMission::g_iReferences--;

	if(CMission::g_iReferences == 0)
	{
		delete CMission::g_pSceneManager;
		CMission::g_pSceneManager = NULL;
	}
}
#pragma endregion

#pragma region Properties
//Return the mission type of this mission object
MissionType CMission::Get_MissionType()
{
	return this->m_pMissionType;
}

//Return the start point
D3DXVECTOR2 CMission::Get_Start()
{
	return this->m_pStart;
}

//Return the goal point
D3DXVECTOR2 CMission::Get_Goal()
{
	return this->m_pGoal;
}

//Return all the enemy groups
EnemyGroup** CMission::Get_EnemyGroups()
{
	return this->m_pEnemyGroups;
}

//Return the mission ID
int CMission::Get_MissionID()
{
	return this->m_iMissionID;
}

//Return the mission name
char* CMission::Get_MissionName()
{
	return this->m_sMissionName;
}
//Return the enemy group at a specific index
EnemyGroup* CMission::Get_EnemyGroupByIndex(int index)
{
	return this->m_pEnemyGroups[index];
}

//Sets the enemy group based on an index passed in
void CMission::Set_EnemyGroupByIndex(EnemyGroup* group, int index)
{
	this->m_pEnemyGroups[index] = group;
}

//Return the spawn number
int CMission::Get_SpawnNumber()
{
	return this->m_iSpawnPointNum;
}

//Set the spawn number
void CMission::Set_SpawnNumber(int value)
{
	this->m_iSpawnPointNum = value;
}

bool CMission::Get_HasWon()
{
	return this->m_bWon;
}
#pragma endregion

#pragma region Methods
//spawn the enemies
void CMission::SpawnEnemy(CGameTime* gameTime)
{
	//Get the ship object
	CQuorumSceneManager* qSceneMgr = (CQuorumSceneManager*)CMission::g_pSceneManager;
	CShip* ship = qSceneMgr->Get_Ship();

	double randX = 0.0;
	double randY = 0.0;

	double distX = 0.0;
	double distY = 0.0;

	if(this->m_bSpawn)
	{
		for(int i = 0; i < this->m_iSpawnPointNum; i++)
		{
			//check to see if there are anymore enemies left in the spawn point
			if(this->m_pEnemyGroups[i]->Number > 0)
			{
				//check to see if the ship is close to the spawn point
				distX = abs(this->m_pEnemyGroups[i]->SpawnPoint.x - qSceneMgr->Get_Ship()->Get_WorldCoord().x);
				distY = abs(this->m_pEnemyGroups[i]->SpawnPoint.y - qSceneMgr->Get_Ship()->Get_WorldCoord().y);

				if(distX <= this->m_pEnemyGroups[i]->Radius && distY <= this->m_pEnemyGroups[i]->Radius)
				{
					//create a new enemy with a random offset from the enemy group spawn point
					randX = ((double)rand()/((double)(RAND_MAX) + (double)1)) * 2 - 1;
					randY = ((double)rand()/((double)(RAND_MAX) + (double)1)) * 2 - 1;
					
					// I'VE BEEN BREAKING HERE REPEATEDLY ... SOMETHING ABOUT Set_Position line.
					qSceneMgr->CreateEnemy(this->m_pEnemyGroups[i]->SpawnPoint + D3DXVECTOR2(randX * 5, randY * 5));

					this->m_pEnemyGroups[i]->Number--;
					this->m_bSpawn = false;
				}
			}
		}
	}
}

//Check the goal state of the mission
bool CMission::Check_GoalState()
{
	CQuorumSceneManager* qSceneMgr = (CQuorumSceneManager*)CMission::g_pSceneManager;

	double x = 0.0;
	double y = 0.0;

	switch (this->m_pMissionType)
	{
		case Gauntlet:
			if(((CQuorumSceneManager*)this->g_pSceneManager)->Get_Ship()->Is_Breached())
			{
				this->m_bWon = false;
				return true;
			}

			x = abs(this->m_pGoal.x - qSceneMgr->Get_Ship()->Get_Position().x);
			y = abs(this->m_pGoal.y - qSceneMgr->Get_Ship()->Get_Position().y);

			if(x <= 10 && y <= 10)
			{
				this->m_bWon = true;
				return true;
			}
			return false;

			break;
		case Defend:
			return false;
			break;
		case Rescue:
			return false;
			break;
	}
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the mission object
void CMission::Initialize()
{
	CGameComponent::Initialize();

	if(CMission::g_pSceneManager == NULL)
		CMission::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");

	if(CMission::g_pSceneManager == NULL)
		exit(-1);
}

//update the mission object
void CMission::Update(CGameTime* gameTime)
{
	CGameComponent::Update(gameTime);

	//update the spawn timer
	if(this->m_fSpawnTimer >= 1.0)
	{
		this->m_bSpawn = true;
	}
	else
	{
		this->m_fSpawnTimer += gameTime->Get_ElapsedTime();
	}
}
#pragma endregion