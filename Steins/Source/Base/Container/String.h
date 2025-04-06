#pragma once

namespace Steins
{
	class String : public std::string
	{
	public:
		String()
			:std::string() {};
		String(std::string _data)
			:std::string(_data) {};
		String(const char* _data)
			:std::string(_data) {};
		~String() {};

	private:
	};
}