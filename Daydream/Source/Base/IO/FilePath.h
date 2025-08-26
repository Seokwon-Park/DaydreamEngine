#pragma once

#include <filesystem>

namespace Daydream
{
	class FilePath
	{
	public:
        FilePath() = default;

        FilePath(const std::string& _path) noexcept
            : path(_path)
        {}

		FilePath(const char* _path) noexcept
			:path(_path)
		{}

        FilePath(const FilePath& other) noexcept = default;
        FilePath& operator=(const FilePath& other) noexcept = default;
        
        FilePath(FilePath&& other) noexcept = default;
        FilePath& operator=(FilePath&& other) noexcept = default;

		inline std::string ToString() const { return path.string(); }
		inline std::wstring ToWString() const { return path.wstring(); }
		inline std::string GetNameFromPath() const { return path.filename().string(); }
		std::string GetFileName() const;
		//inline std::string GetCurrentPath() const { return std::filesystem::current_path().string(); }
		inline std::string GetFileNameWithoutExtension() const { return path.stem().string(); }
		inline String GetExtension() const { return path.extension().string(); }
		inline bool IsDirectory () const { return std::filesystem::is_directory(path); }
		inline bool IsFile() const { return std::filesystem::is_regular_file(path); }
		inline bool IsExist() const { return std::filesystem::exists(path); }
    private:
        std::filesystem::path path;
	};
}

