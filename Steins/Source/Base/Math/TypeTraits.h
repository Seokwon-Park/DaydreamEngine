#pragma once

#include <type_traits>
#include "Base/MathTypes.h"

namespace Steins
{
	template<typename T>
	concept IsValid = std::is_arithmetic_v<T> && std::is_signed_v<T>;

	template<typename From, typename To>
	concept IsCastable = std::is_convertible_v<From, To>;
}
