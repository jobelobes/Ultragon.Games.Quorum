

enum MissionType
{
	Gauntlet = 0,
	Defend = 1,
	Rescue = 2,

};

struct EnemyGroup
{
	D3DXVECTOR2 SpawnPoint;
	float Radius;
	int Number;
};

class QUORUM_COMPONENTS_API CMission : public CGameComponent
{
	private:
		#pragma region Static Variables
		static int g_iReferences;
		static ISceneManager* g_pSceneManager;
		#pragma endregion

		#pragma region Variables
		MissionType m_pMissionType;

		D3DXVECTOR2 m_pStart;
		D3DXVECTOR2 m_pGoal;

		int m_iSpawnPointNum;
		int m_iMissionID;
		char* m_sMissionName;

		EnemyGroup** m_pEnemyGroups;

		//spawn variables
		bool m_bSpawn;
		float m_fSpawnTimer;
		bool m_bWon;
		#pragma endregion

	public:
		#pragma region Constructors and Deconstructors
		/**
		 * Default mission constructor
		 *
		 * @param	game	current game object
		 */
		CMission(CGame* game);

		/**
		 * Parameterized Constructor that takes in start and end points, type, id, and name
		 *
		 * @param	game		current game object
		 * @param	startPoint	start point of the mission
		 * @param	goalPoint	end point of the mission
		 * @param	id			id index of the mission
		 * @param	name		name of the mission
		 * @param	type		type of mission
		 * @param	enemyGroupNum number of enemy unit groups
		 */
		CMission(CGame* game, D3DXVECTOR2 startPoint, D3DXVECTOR2 goalPoint, int id, char* name, MissionType type, int enemySpawnNum);
		/**
		 * Deconstructor method
		 */
		~CMission();
		#pragma endregion

		#pragma region Properties
		/**
		 * Returns mission type of this mission object
		 */
		MissionType Get_MissionType();

		/**
		 * Return the starting point of the mission
		 */
		D3DXVECTOR2 Get_Start();

		/**
		 * Return the goal point of the mission object
		 */
		D3DXVECTOR2 Get_Goal();

		/**
		 * Returns the ID of the mission
		 */
		int Get_MissionID();

		/**
		 * Returns the name of the mission
		 */
		char* Get_MissionName();

		/**
		 * Return the enemy groups
		 */
		EnemyGroup** Get_EnemyGroups();

		/**
		 * Return the enemy group of a specific index
		 *
		 * @param	index	index at which to return the group
		 */
		EnemyGroup* Get_EnemyGroupByIndex(int index);

		/**
		 * Sets the enemy group based on an index
		 *
		 * @param	group	enemy group passed in
		 * @param	index	index of the group in the pointer array
		 */
		void Set_EnemyGroupByIndex(EnemyGroup* group, int index);

		/**
		 * Return the spawn number
		 */
		int Get_SpawnNumber();

		/**
		 * Set the spawn number
		 *
		 * @param	value	parameterized spawn number passed in
		 */
		void Set_SpawnNumber(int value);

		bool Get_HasWon();
		#pragma endregion

		#pragma region Methods
		/**
		 * Spawns the enemies when they are the current mission
		 * Enemies only spawn if the ship is within a certain range of the spawn point
		 *
		 * @param	gameTime	gameTime is passed in to spread out the spawning of enemies
		 */
		void SpawnEnemy(CGameTime* gameTime);
		/**
		 * Checks the goal state of the mission
		 */
		bool Check_GoalState();
		#pragma endregion

		#pragma region Overriden Methods
		/**
		 * Derived from CGameComponent
		 * Initializes the mission object with various properties such as start point, type, and enemy groups
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