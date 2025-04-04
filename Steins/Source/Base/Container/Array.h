#pragma once

#include <xmemory>

namespace Steins
{
	// 임시 구현
	template <typename T, class _Alloc = std::allocator<T>>
	class Array
	{
		UInt32 Size() const { return data.size(); }
		bool IsEmpty() const { return data.empty(); }
		void Pushback(T _t) { data.push_back(_t); }

		inline T operator[](Int32 _Index) { return data[_Index]; }

	private:
		std::vector<T, _Alloc> data;
	};
}

