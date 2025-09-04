#pragma once

#include "Path.h"

namespace Daydream
{
	class Directory
	{
	public:
		Directory(Path _initialPath) :path(_initialPath) {}
		Directory& operator=(std::filesystem::path _path) { path = _path; }

		const Path& getPath() const { return path; }

		std::vector<Path> GetFilePathsWithExtensions(const std::vector<String>& _extensions) const;
	public:
		Path path;
	};
}