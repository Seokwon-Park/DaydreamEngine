#pragma once

namespace Steins
{
	class String
	{
	public:
		String() {};
		~String() {};

	private:
		char* data = nullptr;
		UInt64 size = 0;
	};
}