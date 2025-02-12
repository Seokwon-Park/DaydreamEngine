#pragma once

#include <numbers>

// unsigned integers
using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t; //0-254 0000 0001

// signed integers
using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

using float32 = float;

namespace Steins
{
	constexpr float32 PI = static_cast<float32>(std::numbers::pi);
	constexpr float32 halfPI = PI * 0.5f;
	constexpr float32 two = PI * 2.0f;
	constexpr float32 epsilon = 1e-5f;
}