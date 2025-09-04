#include "DaydreamPCH.h"
#include "Directory.h"

namespace Daydream
{
    Array<Path> Directory::GetFilePathsWithExtensions(const Array<String>& _extensions) const
    {
        Array<Path> results;

        auto list = std::filesystem::recursive_directory_iterator(path.string());

        for (const auto& entry : list)
        {
            if (!entry.is_regular_file())
                continue;
        }
        return Array<Path>();
    }
}
