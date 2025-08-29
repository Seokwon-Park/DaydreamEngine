#pragma once

#include "Path.h"


namespace Daydream
{
	class Directory
	{
		Directory(Path _initialPath) :path(_initialPath) {}

		Array<Path> GetAllFilePaths() const;

		Array<Path> GetFilePathsWithExtension(const String& _extension) const;
	public:
		Path path;
	};
}