#pragma once

#include "TypeCast.h"

namespace Steins
{
	class String 
	{
	public:
		using Iterator = typename std::string::iterator;
		using ConstIterator = typename std::string::const_iterator;

		String()
			:data() {};
		String(std::string _data)
			:data(_data) {};
		String(const char* _data)
			:data(_data) {};
		~String() {};

		inline Int32 Size() const { return StaticCast<Int32>(data.length()); }
		inline bool IsEmpty() const { return data.empty(); }
		inline void PushBack(const char& _element) { data.push_back(_element); }
		inline void Sort() { std::sort(data.begin(), data.end()); }

		inline Iterator begin() { return data.begin(); }
		inline Iterator end() { return data.end(); }
		inline ConstIterator begin() const { return data.begin(); }
		inline ConstIterator end() const { return data.end(); }
		inline char& operator[](Int32 _index) { return data[_index]; }
		inline const char& operator[](Int32 _index) const { return data[_index]; }

		inline const char* Cstr() { return data.c_str(); }

		operator const std::string&() const
		{
			return data;
		}

	private:
		std::string data;
	};

	class StringView
	{
	public:
		StringView()
			:data() {};
		StringView(std::string _data)
			:data(_data) {};
		StringView(const char* _data)
			:data(_data) {};
		~StringView() {};

	private:
		std::string_view data;
	};
}