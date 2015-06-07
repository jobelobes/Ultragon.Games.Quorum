#pragma once
#pragma warning( disable: 4251 )
#pragma warning( disable: 4482 )
#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

#ifdef CORE_API_EXPORTS
	#define CORE_API
#else
	#define CORE_API
#endif

#pragma comment(lib, "Ws2_32.lib")

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <atlbase.h>
#include <limits>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winbase.h>
#include <d3dx10.h>
#include <d3d10.h>
#include <tchar.h>
#include <math.h>

// services
#include "IGameService.h"
#include "INetworkService.h"

#include "CGameTime.h"

#include "CGame.h"
#include "CGameComponent.h"
#include "CDrawableGameComponent.h"
#include "CGameComponentManager.h"
#include "CServiceManager.h"

#include "Packet.h"
#include "CMulticastClient.h"
#include "CNetworkSession.h"

// network
#include "CGameNetworkManager.h"

#include "CGame1.h"