#pragma once
#include <Base/Math/Vector2.h>
#include <Base/Math/Vector4.h>
#include <Base/Math/Color.h>
#include <Base/Container/Array.h>
#include <Base/Container/String.h>

namespace Steins
{
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

	//Type to Type
	template <typename To, typename From>
	To StaticCast(From from)
	{
		return static_cast<To>(from);
	}

	//Pointer To Pointer
	template <typename To, typename From>
	To* StaticCast(From* from)
	{
		return static_cast<To*>(from);
	}

	//Type to Type
	template <typename To, typename From>
	To DynamicCast(From from)
	{
		return dynamic_cast<To>(from);
	}

	//Pointer To Pointer
	template <typename To, typename From>
	To* DynamicCast(From* from)
	{
		return dynamic_cast<To*>(from);
	}

	template <typename To, typename From>
	To PointerCast(From* from)
	{
		return reinterpret_cast<To>(from);
	}

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