#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CQuorumMissionManager : public CGameComponent, public IMissionManager
{
	private:
		#pragma region Variables
		CMission** m_pMissions;
		CMission* m_pCurrMission;

		CGameContent* m_pGameContent;

		int m_iMissionNum;
		bool m_bReachedGoal;
		bool m_bUpdateOk;
		#pragma endregion

	public:

		#pragma region Constructors and Deconstructors
		/**
		 * Default constructor for the mission manager
		 *
		 * @param	game	Current Game Object
		 * @param	gameContent	game content that is loaded at the start
		 */
		CQuorumMissionManager(CGame* game, CGameContent* gameContent);

		/**
		 * Deconstructor method
		 */
		~CQuorumMissionManager();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns the entire array of missions
		 */
		CMission** Get_Missions();

		/**
		 * Returns a mission by index
		 *
		 * @param	index	specific index of the mission to return
		 */
		CMission* Get_MissionByIndex(int index);

		/**
		 * Return the current mission
		 */
		CMission* Get_CurrentMission();

		/**
		 * Set the current mission based on mission number
		 *
		 * @param	index	index reprsenting the level number
		 */
		void Set_CurrentMission(int index); 

		/**
		 * Return if the goal is reached
		 */
		bool Get_IsGoalReached();
		#pragma endregion

		#pragma region Methods
		/**
		 * Loads the missions from the xml file
		 */
		void LoadMissions();

		/**
		 * Loads the enemy groups for the current mission
		 */
		void LoadEnemyGroups();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived from CGameComponent
		 * Initializes the mission manager object with various properties such as start point, type, and enemy groups
		 */
		virtual void Initialize();

		/**
		 * Derived from CGameComponent
		 * Updates the mission status as the game runs
		 *
		 * @param	gameTime	game time of the current update
		 */
		virtual void Update(CGameTime* gameTime);
		#pragma endregion
};