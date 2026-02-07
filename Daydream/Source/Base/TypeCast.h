#pragma once
#include "Wrappers.h"

namespace Daydream
{
	//Type to Type
	template <typename To, typename From>
	To Cast(From&& _from)
	{
		return static_cast<To>(std::forward<From>(_from));
	}

	template<typename To, typename From>
	Shared<To> SharedCast(const Shared<From>& src)
	{
		auto castedPtr = Cast<To*>(src.get());
		return Shared<To>(src, castedPtr);
	}

	//Type to Type
	template <typename To, typename From>
	To SafeCast(From&& _from)
	{
		return dynamic_cast<To>(std::forward<From>(_from));
	}

	template <typename To, typename From>
	To ReinterpretCast(From* from)
	{
		return reinterpret_cast<To>(from);
	}




}