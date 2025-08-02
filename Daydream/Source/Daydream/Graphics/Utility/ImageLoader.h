#pragma once

#include "stb_image.h"

namespace Daydream
{
	namespace ImageLoader
	{
		Array<UInt8> LoadImageFile(const Daydream::FilePath& _path, Int32& _width, Int32& _height, Int32& _channels);
	}
}