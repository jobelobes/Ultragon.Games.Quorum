/**
 * @description 
 *   This class is the preliminary class for loading the xml file that
 *   defines all the games settings, assets, and missions xml.
 *
 * @version 1.0.0.0
 * @author	Josh Wilson
 *
 */

#include "Quorum.Components.h"

struct ITEM
{
	char* assetName;
	D3DXVECTOR2 position;
	char* name;
};

struct LAYER
{
	char* name;
	char* assetName;
	D3DXVECTOR2 worldSize;
	D3DXVECTOR2 mapSize;
	D3DXVECTOR2 tileSize;
	float parallaxSpeed;
	//std::vector<ITEM> items;
};

struct ASSET
{
	char* name;
	char* path;
	char* fileName;
};

struct SYSTEM
{
	char* port;
	char* ip;
	int sfx;
	int music;
	int chat;
	int resolutionWidth;
	int resolutionHeight;
	int maxBullets;
	int maxEnemies;
	RECT world;
};

struct TURRET
{
	char* name;
	int type;
	float cooldown;
	float speed;
	int damage;
	float lifespan;

};

struct BULLET
{
	char* type;
	int range;
	int damage;
	char* texture;
	RECT collision;
};

struct SHIP
{
	int speed;
	int rotation;	
};

struct STATION
{
	int type;
	int id;
	int health;
	int armor;
	int turretType;

};

struct ENEMYGROUP
{
	int missionID;
	int areaX;
	int areaY;
	int radius;
	int number;
};

struct MISSION
{
	int id;
	char* name;
	int type;
	float startX;
	float startY;
	float goalX;
	float goalY;
	int enemyGroupNum;
};

struct ENVIRONMENTOBJECT
{
	int type;
	float scaleX;
	float scaleY;
	float positionX;
	float positionY;
};

class QUORUM_COMPONENTS_API CGameContent
{
	private:
		std::vector<LAYER*> layers;
		std::vector<ASSET*> assets;
		std::vector<SYSTEM*> systemConstants;
		std::vector<TURRET*> turrets;
		std::vector<BULLET*> bullets;
		std::vector<SHIP*> ships;
		std::vector<STATION*> stations;
		std::vector<MISSION*> missions;
		std::vector<ENEMYGROUP*> enemyGroups;
		std::vector<ENVIRONMENTOBJECT*> environmentObjects;
		char* globalPath;

	public:
		#pragma region Static Methods

		/**
		 * Static method that is used to load all content from the xml.
		 *
		 * @param	path	path to the xml file to load
		 */
		static CGameContent* Load(char* path);
		#pragma endregion

		#pragma region Properties

		/**
		 * Gets the global path to the xml file
		 */
		char* Get_GlobalPath();

		/**
		 * Get all the parallax layers defined in the xml file.
		 *
		 * @param	layers	the vector of parallax layers
		 */
		void Get_Layers(std::vector<LAYER*>& layers);

		/**
		 * Get all the graphic assets defined in the xml file
		 *
		 * @param	assets	the vector of assets
		 */
		void Get_Assets(std::vector<ASSET*>& assets);

		/**
		 * Get all the system constants defined in the xml file
		 *
		 * @param	systemConstants	the vector of system constants
		 */
		void Get_SystemConstants(std::vector<SYSTEM*>& systemConstants);

		/**
		 * Get all the ship turrets defined in the xml file
		 *
		 * @param	turrets	the vector of turrets
		 */
		void Get_Turrets(std::vector<TURRET*>& turrets);

		/**
		 * Get all the bullets defined in the xml file
		 *
		 * @param	bullets	the vector of bullets
		 */
		void Get_Bullets(std::vector<BULLET*>& bullets);

		/**
		 * Get all the ships defined in the xml file
		 *
		 * @param	ships	the vector of ships
		 */
		void Get_Ships(std::vector<SHIP*>& ships);

		/**
		 * Get all the ship stations defined in the xml file
		 *
		 * @param	stations	the vector of ship stations
		 */
		void Get_Stations(std::vector<STATION*>& stations);

		/**
		 * Get all the missions defined in the xml file
		 *
		 * @param	missions	the vector of missions
		 */
		void Get_Missions(std::vector<MISSION*>& missions);

		/**
		 * Get all the enemy groups for a mission defined in the xml file
		 *
		 * @param	enemyGroups	the vector of enemy groups
		 */
		void Get_EnemyGroups(std::vector<ENEMYGROUP*>& enemyGroups);

		/**
		 * Get the default station profile for the ship
		 *
		 * @param	stations	the vector of stations
		 */
		void Get_EnemyGroups(std::vector<STATION*>& stations);

		/**
		 * Get all the attribute info for the stations
		 *
		 * @param	turrets	the vector of the turret profiles
		 */
		void Get_EnemyGroups(std::vector<TURRET*>& turrets);

		/**
		 * Get all the environment objects
		 */
		void Get_EnvironmentObjects(std::vector<ENVIRONMENTOBJECT*>& environmentObjects);
		#pragma endregion
};