#pragma once

#include <xmemory>
#include <algorithm>

namespace Steins
{
	// 이 부분은 고민을 좀 해봐야 할듯.
	template <typename T>
	class Array : public std::vector<T>
	{
	public:
		using Iterator = std::vector<T>::iterator;
		using ConstIterator = std::vector<T>::const_iterator;

		Array()
			: std::vector<T>() {}
		Array(Array<T>&& Other)
			: std::vector<T>(std::move(Other)) {}
		Array(const Array<T>& Other)
			: std::vector<T>(Other) {}
		Array(std::initializer_list<T> l)
			: std::vector<T>(l) {}

		Int32 Size() const { return StaticCast<Int32>(std::vector<T>::size()); }
		bool IsEmpty() const { return std::vector<T>::empty(); }
		void Pushback(const T& _element) { std::vector<T>::push_back(_element); }
		void Sort() { std::sort(std::vector<T>::begin(), std::vector<T>::end()); }
	private:
	};
}

