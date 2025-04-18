#pragma once

#pragma warning(disable: 4819)
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>

#include <string>
#include <vector>
#include <string_view>
#include <set>
#include <unordered_map>
#include <glm/glm.hpp>
#include <DirectXMath.h>

#include "Steins/Core/Logger.h"
#include "Steins/Core/Assert.h"
#include "Base/Base.h"


#if defined(STEINS_PLATFORM_WINDOWS)
	#define GLFW_EXPOSE_NATIVE_WIN32
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // !NOMINMAX
	#include <Windows.h>
	#include <windowsx.h>

	#include <wrl.h> 
	#include <d3d11.h>
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <dxgidebug.h>
	#include <d3dcompiler.h>
	#include <DirectXCollision.h>

	#pragma comment(lib, "d3d11")
	#pragma comment(lib, "d3d12")
	#pragma comment(lib, "d3dcompiler") 
	#pragma comment(lib, "DXGI") 
	#pragma comment(lib, "dxguid") 

	using namespace Microsoft::WRL;
#endif