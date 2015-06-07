#pragma once

#include "3ngine.Core.h"

#ifdef EXTENSIONS_API_EXPORTS
	#define EXTENSIONS_API __declspec(dllexport)
#else
	#define EXTENSIONS_API __declspec(dllimport)
#endif

#include "ISceneManager.h"
#include "IScene.h"
#include "ICollidableUnit.h"
#include "ICollisionService.h"
#include "IControllableUnit.h"
#include "IController.h"

#include "CSceneManager.h"
#include "CCollisionManager.h"

#include "CSquare.h"

