#pragma once

namespace Daydream
{
	//Type to Type
	template <typename To, typename From>
	To Cast(From&& from)
	{
		return static_cast<To>(std::forward<From>(from));
	}

	////Pointer To Pointer
	template <typename To, typename From>
	To* Cast(From* from)
	{
		return static_cast<To*>(from);
	}

	//Type to Type
	template <typename To, typename From>
	To SafeCast(From&& from)
	{
		return dynamic_cast<To>(std::forward<From>(from));
	}

	//Pointer To Pointer
	template <typename To, typename From>
	To* SafeCast(From* from)
	{
		return dynamic_cast<To*>(from);
	}

	template <typename To, typename From>
	To ReinterpretCast(From* from)
	{
		return reinterpret_cast<To>(from);
	}
}