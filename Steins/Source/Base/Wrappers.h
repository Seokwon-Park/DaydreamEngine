#pragma once

#include "Math.h"
#include "MathTypes.h"
#include <Base/TypeCast.h>
#include <Base/IO/FilePath.h>
#include <Base/Math/Vector2.h>
#include <Base/Math/Vector3.h>

namespace Steins
{
	template <typename T1, typename T2>
	using Pair = std::pair<T1, T2>;

	template <typename T1, typename T2>
	constexpr Pair<T1, T2> MakePair(T1&& _first, T2&& _second)
	{
		return std::make_pair<T1, T2>(std::forward<T1>(_first), std::forward<T2>(_second));
	}

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Unique<T> MakeUnique(Args&& ... _args)
	{
		return std::make_unique<T>(std::forward<Args>(_args)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Shared<T> MakeShared(Args&& ... _args)
	{
		return std::make_shared<T>(std::forward<Args>(_args)...);
	}

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;

	using Vector3i = Vector2<int>;
	using Vector3f = Vector2<float>;
	using Vector3d = Vector2<double>;
}