#pragma once

#include "unordered_map"

namespace Steins
{
	template <typename Key, typename Value>
	using HashMap = std::unordered_map<Key, Value>;
}