#pragma once

#pragma warning(disable: 4819)

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>

#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>

#include "Steins/Core/Logger.h"
#include "Steins/Core/Assert.h"
#include "Base/MathTypes.h"
#include "Base/Wrappers.h"


#if defined(STEINS_PLATFORM_WINDOWS)
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // !NOMINMAX
	#include <Windows.h>
	#include <windowsx.h>

	#include <wrl.h> 
	#include <d3d11.h>
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <DirectXCollision.h>

	#pragma comment(lib, "d3d11")
	#pragma comment(lib, "d3d12")
	#pragma comment(lib, "d3dcompiler") 
	#pragma comment(lib, "DXGI") 

	using namespace Microsoft::WRL;
#endif