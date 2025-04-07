#pragma once

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
}