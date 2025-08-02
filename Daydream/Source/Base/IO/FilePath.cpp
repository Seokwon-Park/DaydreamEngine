#include "DaydreamPCH.h"
#include "FilePath.h"

namespace Daydream
{
	inline std::string FilePath::GetFileName() const
	{
		Daydream_CORE_ASSERT(IsDirectory(), ToString() + "is not a Directory");

		return path.filename().string();
	}
}