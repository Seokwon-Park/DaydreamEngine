#pragma once

#include <filesystem>

namespace Daydream
{
	namespace FileSystem = std::filesystem;
	using Path = FileSystem::path;

	//class Path
	//{
	//public:
	//	Path() = default;
	//	Path(const std::filesystem::path& p) : path(p) {}
	//	Path(std::filesystem::path&& p) : path(std::move(p)) {}
	//	Path(std::string_view str) : path(str) {}
	//	Path(const std::string& str) : path(str) {}
	//	Path(const char* c_str) : path(c_str) {}

	//	Path(const Path& other) noexcept = default;
	//	Path& operator=(const Path& other) noexcept = default;

	//	Path(Path&& other) noexcept = default;
	//	Path& operator=(Path&& other) noexcept = default;

	//	Path operator+(const char* _path)
	//	{
	//		Path result = *this;
	//		result.path.append(_path);
	//		return result;
	//	}

	//	Path& operator/=(const char* _path)
	//	{
	//		path /= " shibal"
	//	}


	//	inline String ToString() const { return path.string(); }
	//	inline WideString ToWString() const { return path.wstring(); }

	//	inline String GetFileName() const
	//	{
	//		return GetLastComponentName();
	//	}

	//	inline String GetDirectoryName() const
	//	{
	//		return GetLastComponentName();
	//	}

	//	inline String GetLastComponentName() const { return path.filename().string(); }
	//	
	//	inline String GetFileNameWithoutExtension() const 
	//	{
	//		return path.stem().string(); 
	//	}

	//	inline void MoveToParent() { path = path.parent_path(); }

	//	inline String GetExtension() const { return path.extension().string(); }
	//	inline String GetAbsolutePath() const { return std::filesystem::absolute(path).string(); }
	//	inline bool IsDirectory() const { return std::filesystem::is_directory(path); }
	//	inline bool IsFile() const { return std::filesystem::is_regular_file(path); }
	//	inline bool IsExistPath() const { return std::filesystem::exists(path); }
	//private:
	//	std::filesystem::path path;
	//};
}

