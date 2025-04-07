#pragma once

#include <xmemory>
#include <algorithm>

namespace Steins
{
	// 이 부분은 고민을 좀 해봐야 할듯.
	template <typename T, class Alloc = std::allocator<T>>
	class Array
	{
	public:
		using Iterator = typename std::vector<T, Alloc>::iterator;
		using ConstIterator = typename std::vector<T, Alloc>::const_iterator;

		Array()
			: data(std::vector<T>()) {}
		Array(Array<T>&& _other)
			: data(std::move(_other.data)) {}
		Array(const Array<T>& _other)
			: data(_other.data) {}
		Array(std::initializer_list<T> _initList)
			: data(_initList) {}
		Array<T>& operator=(const Array<T>& _other)
		{
			data = _other.data;
			return *this;
		};
		Array<T>& operator=(Array<T>&& _other) noexcept
		{
			data = std::move(_other.data);
			return *this;
		}

		inline Int32 Size() const { return StaticCast<Int32>(data.size()); }
		inline bool IsEmpty() const { return data.empty(); }
		inline void PushBack(const T& _element) { data.push_back(_element); }
		inline void Sort() { std::sort(data.begin(), data.end()); }
		inline Iterator begin() { return data.begin(); }
		inline Iterator end() { return data.end(); }
		inline ConstIterator begin() const { return data.begin(); }
		inline ConstIterator end() const { return data.end(); }
		inline T& operator[](Int32 _index) { return data[_index]; }
		inline const T& operator[](Int32 _index) const { return data[_index]; }
	private:
		std::vector<T, Alloc> data;
	};
}

