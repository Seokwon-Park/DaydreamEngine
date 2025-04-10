#include "SteinsPCH.h"
#include "FilePath.h"

namespace Steins
{
	inline std::string FilePath::GetFileName() const
	{
		STEINS_CORE_ASSERT(IsDirectory(), ToString() + "is not a Directory");

		return path.filename().string();
	}
}