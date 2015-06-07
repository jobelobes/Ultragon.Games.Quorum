#pragma once

#ifdef GRAPHICS2D_API_EXPORTS
#define GRAPHICS2D_API __declspec(dllexport)
#else
#define GRAPHICS2D_API __declspec(dllimport)
#endif

#include "3ngine.Core.h"

#include "CAxis.h"
#include "CTile.h"
#include "CCube.h"
#include "CTiledPlane.h"
