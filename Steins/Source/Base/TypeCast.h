#pragma once

namespace Steins
{
	//Type to Type
	template <typename To, typename From>
	To StaticCast(From&& from)
	{
		return static_cast<To>(std::forward<From>(from));
	}

	////Pointer To Pointer
	template <typename To, typename From>
	To* StaticCast(From* from)
	{
		return static_cast<To*>(from);
	}

	//Type to Type
	template <typename To, typename From>
	To DynamicCast(From&& from)
	{
		return dynamic_cast<To>(std::forward<From>(from));
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
}