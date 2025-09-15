#include "DaydreamPCH.h"
#include "ImageLoader.h"
#include "stb_image.h"

namespace Daydream::ImageLoader
{
	ImageData LoadImageFile(const Path& _path)
	{
		if (stbi_is_hdr(_path.string().c_str()))
		{
			return LoadAndFillImageData<Float32>(_path);
		}
		else
		{
			return LoadAndFillImageData<stbi_uc>(_path);
		}
		return ImageData();
	}




	HDRIImageData LoadHDRIFile(const Path& _path)
	{
		HDRIImageData imageData;

		int channel;
		Float32* pixels = stbi_loadf(_path.string().c_str(), &imageData.width, &imageData.height, &channel, 0);
		DAYDREAM_CORE_ASSERT(pixels, "Failed to load image!");

		UInt32 imageSize = imageData.width * imageData.height;
		imageData.data.resize(imageSize * 4);
		if (channel == 3)
		{
			for (int i = 0; i < imageSize; i++)
			{
				imageData.data[i * 4] = pixels[i * 3];
				imageData.data[i * 4 + 1] = pixels[i * 3 + 1];
				imageData.data[i * 4 + 2] = pixels[i * 3 + 2];
				imageData.data[i * 4 + 3] = 1.0f;
			}
		}
		else if (channel == 4)
		{
			memcpy(imageData.data.data(), pixels, imageSize * 4);
		}
		stbi_image_free(pixels);

		return imageData;
	}
}