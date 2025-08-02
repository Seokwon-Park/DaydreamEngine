#pragma once

#include "unordered_map"

namespace Daydream
{
	template <typename Key, typename Value>
	using HashMap = std::unordered_map<Key, Value>;
}