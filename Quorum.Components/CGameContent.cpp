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

#pragma region Static Methods
CGameContent* CGameContent::Load(char* path)
{
	CGameContent* gameContent = new CGameContent();
	
	const WCHAR* pwszLocalName;
    const WCHAR* pwszValue;

	XmlNodeType nodeType;
	IStream* stream;
	IXmlReader* reader;
	::SHCreateStreamOnFile(path, STGM_READ, &stream);
	::CreateXmlReader(__uuidof(IXmlReader), (void**)&reader, NULL);
	reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	reader->SetInput(stream);

	while(S_OK == reader->Read(&nodeType))
	{
		switch(nodeType)
		{
		case XmlNodeType_XmlDeclaration:
			break;
		case XmlNodeType_Element:
			if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
			{
				if(wcscmp(pwszLocalName,L"assets") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"path") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										gameContent->globalPath = temp;
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
								break;
						}
					}					
				}
				else if (wcscmp(pwszLocalName,L"asset") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						ASSET* asset = new ASSET();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"name") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										asset->name = temp;
									}
									else if(wcscmp(pwszLocalName,L"path") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										asset->path = temp;
									}
									else if(wcscmp(pwszLocalName,L"filename") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										asset->fileName = temp;
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->assets.push_back(asset);
								break;
							}

						}
					}
				}
				else if (wcscmp(pwszLocalName,L"layer") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						LAYER* layer = new LAYER();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"name") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										layer->name = temp;
									}
									else if(wcscmp(pwszLocalName,L"assetName") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										layer->assetName = temp;
									}
									else if(wcscmp(pwszLocalName,L"parallax") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										layer->parallaxSpeed = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"mapSizeX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										layer->mapSize.x = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"mapSizeY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										layer->mapSize.y = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"tileSizeX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										layer->tileSize.x = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"tileSizeY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										layer->tileSize.y = _wtof(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->layers.push_back(layer);
								break;
							}

						}
					}
				}
				else if(wcscmp(pwszLocalName,L"system") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						SYSTEM* systemConstants = new SYSTEM();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"maxBullets") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->maxBullets = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"maxEnemies") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->maxEnemies = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"worldX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->world.left = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"worldY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->world.top = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"worldWidth") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->world.right = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"worldHeight") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->world.bottom = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"port") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										systemConstants->port = temp;
									}
									else if(wcscmp(pwszLocalName,L"ip") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										systemConstants->ip = temp;
									}
									else if(wcscmp(pwszLocalName,L"sfx") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->sfx = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"music") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->music = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"chat") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->chat = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"resolutionWidth") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->resolutionWidth = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"resolutionHeight") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										systemConstants->resolutionHeight = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"turret") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						TURRET* turret = new TURRET();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"name") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										turret->name = temp;
									}
									else if(wcscmp(pwszLocalName,L"type") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										turret->type = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"cooldown") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										turret->cooldown = _wtof(pwszValue); 
									}
									else if(wcscmp(pwszLocalName,L"speed") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										turret->speed = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"damage") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										turret->damage = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"lifespan") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										turret->lifespan = _wtof(pwszValue);
									}
									
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->turrets.push_back(turret);
								break;
							}

						}
					}
				}
				else if(wcscmp(pwszLocalName,L"bullet") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						BULLET* bullet = new BULLET();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"type") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										bullet->type = temp;
									}
									else if(wcscmp(pwszLocalName,L"range") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->range = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"damage") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->damage = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"texture") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										bullet->texture = temp;
									}
									else if(wcscmp(pwszLocalName,L"collisionX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->collision.left = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"collisionY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->collision.top = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"collisionWidth") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->collision.right = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"collisionHeight") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										bullet->collision.bottom = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->bullets.push_back(bullet);
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"ship") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						SHIP* ship = new SHIP();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"speed") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										ship->speed = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"rotation") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										ship->rotation = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"station") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						STATION* station = new STATION();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"type") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										station->type = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"id") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										station->id = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"health") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										station->health = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"armor") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										station->armor = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"turretType") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										station->turretType = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->stations.push_back(station);
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"mission") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						MISSION* mission = new MISSION();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"name") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);
										mission->name = temp;
									}
									else if(wcscmp(pwszLocalName,L"startX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->startX = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"startY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->startY = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"goalX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->goalX = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"goalY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->goalY = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName, L"id") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->id = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName, L"type") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->type = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName, L"enemyGroupNum") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										mission->enemyGroupNum = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->missions.push_back(mission);
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"enemyGroup") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						ENEMYGROUP* enemyGroup = new ENEMYGROUP();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"missionID") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										enemyGroup->missionID = _wtoi(pwszValue);
									}
									if(wcscmp(pwszLocalName,L"areaX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										enemyGroup->areaX = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"areaY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										enemyGroup->areaY = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"radius") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										enemyGroup->radius = _wtoi(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"number") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										enemyGroup->number = _wtoi(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->enemyGroups.push_back(enemyGroup);
								break;
							}
						}
					}
				}
				else if(wcscmp(pwszLocalName,L"environmentObject") == 0)
				{
					if(!FAILED(reader->MoveToFirstAttribute()))
					{
						ENVIRONMENTOBJECT* environmentObject = new ENVIRONMENTOBJECT();

						while(TRUE)
						{
							if(!reader->IsDefault())
							{
								if(!FAILED(reader->GetLocalName(&pwszLocalName, NULL)))
								{
									if(wcscmp(pwszLocalName,L"type") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										size_t i = wcsnlen(pwszValue, 100);
										char* temp = (char*)malloc(i + 1);
										size_t k;
										wcstombs_s(&k, temp, i + 1, pwszValue, i + 1);

										int value = 0;
										if(strcmp(temp, "Asteroid") == 0)
											value = 0;
										else if(strcmp(temp, "AsteroidNormal") == 0)
											value = 1;
										else if(strcmp(temp, "Planet") == 0)
											value = 2;
										else if(strcmp(temp, "SpaceStation") == 0)
											value = 3;
										else if(strcmp(temp, "Nebula") == 0)
											value = 4;

										environmentObject->type = value;
									}
									else if(wcscmp(pwszLocalName,L"scaleX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										environmentObject->scaleX = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"scaleY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										environmentObject->scaleY = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"positionX") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										environmentObject->positionX = _wtof(pwszValue);
									}
									else if(wcscmp(pwszLocalName,L"positionY") == 0)
									{
										reader->GetValue(&pwszValue, NULL);
										environmentObject->positionY = _wtof(pwszValue);
									}
								}
							}
							if (S_OK != reader->MoveToNextAttribute())
							{
								gameContent->environmentObjects.push_back(environmentObject);
								break;
							}
						}
					}
				}
			}
			break;
		case XmlNodeType_EndElement:

			break;
		case XmlNodeType_Text:
			break;
		case XmlNodeType_Whitespace:
			break;
		case XmlNodeType_CDATA:
			break;
		case XmlNodeType_ProcessingInstruction:
			break;
		case XmlNodeType_Comment:
			break;
		case XmlNodeType_DocumentType:
			break;
		}
	}
	
	return gameContent;
}
#pragma endregion

#pragma region Properties
char* CGameContent::Get_GlobalPath()
{
	return this->globalPath;
}
	
void CGameContent::Get_Layers(std::vector<LAYER*>& outLayers)
{
	outLayers = layers;
}
	
void CGameContent::Get_Assets(std::vector<ASSET*>& outAssets)
{
	outAssets = assets;
}

void CGameContent::Get_Missions(std::vector<MISSION*>& outMissions)
{
	outMissions = missions;
}

void CGameContent::Get_EnemyGroups(std::vector<ENEMYGROUP*>& outEnemyGroups)
{
	outEnemyGroups = enemyGroups;
}

void CGameContent::Get_Stations(std::vector<STATION*>& outStations)
{
	outStations = stations;
}

void CGameContent::Get_Turrets(std::vector<TURRET*>& outTurrets)
{
	outTurrets = turrets;
}

void CGameContent::Get_EnvironmentObjects(std::vector<ENVIRONMENTOBJECT*>& outEnvObjects)
{
	outEnvObjects = environmentObjects;
}
#pragma endregion