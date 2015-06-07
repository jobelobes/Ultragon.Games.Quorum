#include "Quorum.Components.h"

#pragma region Constructors and Deconstructors
//Default constructor
CQuorumMissionManager::CQuorumMissionManager(CGame *game, CGameContent* gameContent)
: CGameComponent(game)
{
	game->Get_Services()->AddService(this);

	this->m_pMissions = NULL;
	this->m_pCurrMission = NULL;

	this->m_pGameContent = gameContent;

	this->m_iMissionNum = 0;
	this->m_bUpdateOk = false;
	this->m_bReachedGoal = false;
}

//Deconstructor
CQuorumMissionManager::~CQuorumMissionManager()
{

}
#pragma endregion

#pragma region Properties
//Return the entire array of missions
CMission** CQuorumMissionManager::Get_Missions()
{
	return this->m_pMissions;
}

//Return a specific mission by index
CMission* CQuorumMissionManager::Get_MissionByIndex(int index)
{
	return this->m_pMissions[index];
}

//Return the current mission
CMission* CQuorumMissionManager::Get_CurrentMission()
{
	return this->m_pCurrMission;
}

//Set the current mission by index representing level number
void CQuorumMissionManager::Set_CurrentMission(int index)
{
	this->m_pCurrMission = this->m_pMissions[index];

	this->LoadEnemyGroups();

	//set the flag to make it ok to update
	this->m_bUpdateOk = true;
}

bool CQuorumMissionManager::Get_IsGoalReached()
{
	return this->m_bReachedGoal;
}
#pragma endregion

#pragma region Methods
//Load the missions into the manager
void CQuorumMissionManager::LoadMissions()
{
	//load the game content from the game content manager
	std::vector<MISSION*> missions;
	this->m_pGameContent->Get_Missions(missions);

	//set the number of missions for future reference
	this->m_iMissionNum = missions.size();

	//initialise the missions array
	this->m_pMissions = new CMission*[this->m_iMissionNum];

	for(int i = 0; i < this->m_iMissionNum; i++)
	{
		this->m_pMissions[i] = new CMission(this->Get_Game(),
			D3DXVECTOR2(missions[i]->startX, missions[i]->startY),
			D3DXVECTOR2(missions[i]->goalX, missions[i]->goalY),
			missions[i]->id,
			missions[i]->name,
			(MissionType)missions[i]->type,
			missions[i]->enemyGroupNum);
		this->Get_Game()->Get_Components()->Add(this->m_pMissions[i]);
	}
}

//Load the enemy groups for the current mission
void CQuorumMissionManager::LoadEnemyGroups()
{
	std::vector<ENEMYGROUP*> enemyGroups;
	this->m_pGameContent->Get_EnemyGroups(enemyGroups);

	int tempIndex = 0;
	for(int i = 0; i < enemyGroups.size(); i++)
	{
		if(enemyGroups[i]->missionID == this->m_pCurrMission->Get_MissionID())
		{
			EnemyGroup* tmp = new EnemyGroup();
			tmp->SpawnPoint = D3DXVECTOR2(enemyGroups[i]->areaX, enemyGroups[i]->areaY);
			tmp->Radius = (float)enemyGroups[i]->radius;
			tmp->Number = enemyGroups[i]->number;

			this->m_pCurrMission->Set_EnemyGroupByIndex(tmp, tempIndex);

			tempIndex++;
		}
	}
}
#pragma endregion

#pragma region Overriden Methods
//Initialize the manager
void CQuorumMissionManager::Initialize()
{
	CGameComponent::Initialize();

	this->LoadMissions();
}

//Update the manager
void CQuorumMissionManager::Update(CGameTime* gameTime)
{	
	CGameComponent::Update(gameTime);

	if(this->m_bUpdateOk)
	{
		//making sure all the missions that aren't the current are disabled so they don't get updated
		for(int i = 0; i < this->m_iMissionNum; i++)
		{
			if(this->m_pCurrMission != this->m_pMissions[i])
				this->m_pMissions[i]->Set_Enabled(false);
		}

		if(this->m_pCurrMission->Check_GoalState())
		{
			//Set goal to true
			this->m_bReachedGoal = true;
		}

		//if the goal state of the current mission hasn't been met, spawn enemies when appropriate
		if(!this->m_pCurrMission->Check_GoalState() && this->m_pCurrMission != NULL)
			this->m_pCurrMission->SpawnEnemy(gameTime);
	}
}
#pragma endregion
