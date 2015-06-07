#pragma once
#pragma warning( disable: 4251 )
#pragma warning( disable: 4482 )
#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

#ifdef CORE_API_EXPORTS
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "Ws2_32.lib")

#include <vector>
#include <list>
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
#include <time.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <dxerr.h>
#include <winbase.h>
#include <d3dx10.h>
#include <d3d10.h>
#include <dinput.h>
#include <XInput.h>
#include <xact3.h>
#include <tchar.h>
#include <math.h>

// Utilities
#include "CObjLoader.h"

// services
#include "IGameService.h"
#include "IInputManager.h"
#include "IInputService.h"
#include "IWindowsInputService.h"
#include "IXBoxInputService.h"
#include "ICameraService.h"
#include "IXACTEngineService.h"
#include "INetworkService.h"

// Vertex Declarations
#include "VertexPositionColor.h"
#include "VertexPositionColorTexture.h"
#include "VertexPositionNormalTexture.h"

#include "CGameTime.h"
#include "CGame.h"
#include "CGraphicsDevice.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CGameWindow.h"


#include "CGameComponent.h"
#include "CDrawableGameComponent.h"
#include "CGameComponentManager.h"
#include "CServiceManager.h"

#include "Packet.h"
#include "CMulticastClient.h"
#include "CNetworkSession.h"

#include "CEffect.h"
#include "CTexture2D.h"
#include "CSprite2D.h"
#include "CSpriteBatch.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimation.h"

#include "CBoundingBox.h"
#include "CBoundingPolygon.h"

// input
#include "CWindowsInput.h"
#include "CXBoxInput.h"
#include "CInputManager.h"

// network
#include "CGameNetworkManager.h"

// camera
#include "CArcBallCamera.h"
#include "CTopDownCamera.h"

// audio
#include "XACTEngine.h"

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)													\
		{															\
			HRESULT hr = (x);										\
			if(FAILED(hr))											\
			{														\
				DXTRACE(__FILE__, (DWORD)__LINE__, hr, L#x, true);	\
			}														\
		}
	#endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif