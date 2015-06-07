#pragma once

#ifdef QUORUM_COMPONENTS_API_EXPORTS
#define QUORUM_COMPONENTS_API __declspec(dllexport)
#else
#define QUORUM_COMPONENTS_API __declspec(dllimport)
#endif

#pragma comment(lib, "XmlLite.lib")

//Core engine components and extensions
#include "3ngine.Core.h"
#include "3ngine.Graphics2D.h"
#include "3ngine.Extensions.h"

//XML Loader
#include "XmlLite.h"
#include "CMission.h"

//Component interfaces / services
#include "IWorldScroller.h"
#include "IScreenManager.h"
#include "IMissionManager.h"
#include "IBasicUnit.h"

//Quorum Component base classes
#include "CQuorumIdentifiers.h"
#include "CGameUnit.h"
#include "CGameScreen.h"
#include "CScreenManager.h"
#include "CNinePatch.h"
#include "CGameContent.h"

//Quorum World environment classes
#include "CQuorumCamera.h"
#include "CParallaxBackground.h"
#include "CWorldBackground.h"

//Unit / Ship component classes

#include "CBullet.h"
#include "CShield.h"
#include "CTarget.h"
#include "CTurret.h"
#include "CLCDisplay.h"
#include "CFloorHealth.h"
#include "CStationHUD.h"
#include "CStation.h"
#include "CShip.h"
#include "CEnvironmentObject.h"
#include "CCrew.h"
#include "CEnemyUnit.h"

//Local user station controller classes
#include "CShieldController.h"
#include "CTurretController.h"
#include "CBasicUnitController.h"

//Player / AI / Network controller classes
#include "CBaseEnemyController.h"
#include "CAIEnemyController.h"
#include "CNetworkEnemyController.h"
#include "CBaseCrewController.h"
#include "CPlayerCrewController.h"
#include "CNetworkCrewController.h"

//Manager classes
#include "CQuorumMissionManager.h"
#include "CQuorumNetworkManager.h"
#include "CQuorumSceneManager.h"
