#pragma once

#include "stb_image.h"

namespace Daydream::ImageLoader
{
	struct ImageData
	{
		Int32 width;
		Int32 height;
		Array<UInt8> data;
	};

	struct HDRIImageData
	{
		Int32 width;
		Int32 height;
		Array<Float32> data;
	};

	ImageData LoadImageFile(const Path& _path);
	HDRIImageData LoadHDRIFile(const Path& _path);
}