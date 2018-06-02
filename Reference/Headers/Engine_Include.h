#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Engine_Typedef.h"
#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#pragma warning(disable : 4251)

#include <crtdbg.h>
#include <assert.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif