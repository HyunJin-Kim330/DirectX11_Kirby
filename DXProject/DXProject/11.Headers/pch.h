#pragma once

#include "Types.h"
#include "Values.h"
#include "Structs.h"

#include <exception>

// STL
#include <algorithm>
#include <vector>
#include <memory>
#include <list>
#include <array>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <iostream> 
#include <unordered_map>

using namespace std;

// WIN
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif


// PROJECT
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Transform.h"
#include "Component.h"
#include "GameObject.h"
#include "BlendState.h"
#include "ObjectPool.h"

#define CHECK(p)		assert(SUCCEEDED(p))
#define	GAME			GGameManager
#define Time			GAME->GetTimeManager()
#define Input			GAME->GetInputManager()
#define Pool				GAME->GetObjectPool()

#include "tinyxml2.h"
using namespace tinyxml2;