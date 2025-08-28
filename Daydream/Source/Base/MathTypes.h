#pragma once

#include <numbers>



// unsigned integers
using UInt64 = uint64_t;
using UInt32 = uint32_t;
using UInt16 = uint16_t;
using UInt8 = uint8_t;

// signed integers
using Int64 = int64_t;
using Int32 = int32_t;
using Int16 = int16_t;
using Int8 = int8_t;

using Bool = bool;

using Float32 = float;

namespace Daydream
{
	constexpr Float32 PI = static_cast<Float32>(std::numbers::pi);
	constexpr Float32 halfPI = PI * 0.5f;
	constexpr Float32 twoPI = PI * 2.0f;
	constexpr Float32 epsilon = 1e-5f;
}