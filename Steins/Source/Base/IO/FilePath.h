#pragma once

#include <filesystem>

namespace Steins
{
	class FilePath
	{
	public:
		FilePath(std::string _path) 
			:path(_path) {}

		FilePath(String _path)
			:path(std::string(_path)) {}

		inline String ToString() const { return path.string(); }
    private:
        std::filesystem::path path;
	};
}

