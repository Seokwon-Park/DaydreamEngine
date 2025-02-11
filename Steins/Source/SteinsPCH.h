#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

#if defined(STEINS_PLATFORM_WINDOWS)
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif // !NOMINMAX

#include <Windows.h>
#include <wrl.h> 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#endif