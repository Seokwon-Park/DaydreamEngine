#pragma once

#include <xmemory>

namespace Steins
{
	template <typename T, class _Alloc = std::allocator<T>>
	class Array
	{
		UInt32 Size() const { return data.size(); }
		bool IsEmpty() const { return data.empty(); }
		void Pushback(T _t) { data.push_back(_t); }

	private:
		std::vector<T, _Alloc> data;
	};
}

