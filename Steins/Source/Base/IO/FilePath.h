#pragma once

#include <filesystem>

namespace Steins
{
	class FilePath
	{
	public:
		FilePath(std::string _path) 
			:path(_path) {}

		inline std::string ToString() const { return path.string(); }
		inline std::string GetNameFromPath() const { return path.filename().string(); }
		inline std::string GetFileName() const;
		inline std::string GetFileNameWithoutExtension() const { return path.stem().string(); }
		inline bool IsDirectory () const { return std::filesystem::is_directory(path); }
		inline bool IsFile() const { return std::filesystem::is_regular_file(path); }
		inline bool IsExist() const { return std::filesystem::exists(path); }
    private:
        std::filesystem::path path;
	};
}

