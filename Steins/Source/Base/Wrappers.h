#pragma once
#include <Base/Math/Vector2.h>
#include <Base/Math/Vector4.h>
#include <Base/Math/Color.h>
#include <Base/TypeCast.h>
#include <Base/Container/Array.h>
#include <Base/Container/String.h>
#include <Base/Container/SmartPointer.h>
#include <Base/IO/FilePath.h>

namespace Steins
{
	template <typename T1, typename T2>
	using Pair = std::pair<T1, T2>;

	template <typename T1, typename T2>
	constexpr Pair<T1, T2> MakePair(T1&& _first, T2&& _second)
	{
		return std::make_pair<T1, T2>(std::forward<T1>(_first), std::forward<T2>(_second));
	}

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;

	using Vector4i = Vector4<int>;
	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;

}