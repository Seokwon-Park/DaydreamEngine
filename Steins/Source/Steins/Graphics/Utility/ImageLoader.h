#pragma once

#include "stb_image.h"

namespace Steins
{
	namespace ImageLoader
	{
		Array<UInt8> LoadImageFile(const Steins::FilePath& _path, Int32& _width, Int32& _height, Int32& _channels);
	}
}