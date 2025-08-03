#include "DaydreamPCH.h"
#include "ImageLoader.h"
#include "stb_image.h"

namespace Daydream
{
	Array<UInt8> ImageLoader::LoadImageFile(const Daydream::FilePath& _path, Int32& _width, Int32& _height, Int32& _channels)
	{
		
		stbi_uc* data = stbi_load(_path.ToString().c_str(), &_width, &_height, &_channels, 0);
		DAYDREAM_CORE_ASSERT(data, "Failed to load image!");

		Array<UInt8> newPixels(_width * _height * 4);
		if (_channels == 3)
		{
			for (int i = 0; i < _width * _height; i++)
			{
				newPixels[i * 4] = data[i * 3];
				newPixels[i * 4 + 1] = data[i * 3 + 1];
				newPixels[i * 4 + 2] = data[i * 3 + 2];
				newPixels[i * 4 + 3] = 255;
			}
		}
		else if (_channels == 4)
		{
			memcpy(newPixels.data(), data, _width * _height * 4);
		}
		stbi_image_free(data);

		return newPixels;
	}
}