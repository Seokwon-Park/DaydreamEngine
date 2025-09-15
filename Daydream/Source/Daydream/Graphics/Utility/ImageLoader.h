#pragma once

#include "stb_image.h"

namespace Daydream::ImageLoader
{
	struct ImageData
	{
		Int32 width = 0;
		Int32 height = 0;
		std::variant<Array<UInt8>, Array<Float32>> data;

		const void* GetRawDataPtr()
		{
			return std::visit([](const auto& array) -> const void* 
				{
					return array.data();
				}, data);
		}
	};

	struct HDRIImageData
	{
		Int32 width;
		Int32 height;
		Array<Float32> data;
	};

	template<typename T>
	ImageData LoadAndFillImageData(const Path& _path)
	{
		ImageData imageData;
		T* pixels = nullptr;
		if constexpr (std::is_same_v<T, stbi_uc>) { // T�� unsigned char���
			pixels = stbi_load(_path.string().c_str(), &imageData.width, &imageData.height, nullptr, 4); // 4ä�η� ����
		}
		else
		{ // T�� float���
			pixels = stbi_loadf(_path.string().c_str(), &imageData.width, &imageData.height, nullptr, 4); // 4ä�η� ����
		}

		DAYDREAM_CORE_ASSERT(pixels, "Failed to load image!");

		using ArrayType = Array<std::conditional_t<std::is_same_v<T, stbi_uc>, UInt8, Float32>>;
		imageData.data = ArrayType();
		auto& pixelData = std::get<ArrayType>(imageData.data);

		size_t imageSize = (size_t)imageData.width * imageData.height * 4;
		pixelData.resize(imageSize);
		memcpy(pixelData.data(), pixels, imageSize * sizeof(T));

		stbi_image_free(pixels);

		return imageData;
	}

	ImageData LoadImageFile(const Path& _path);
	HDRIImageData LoadHDRIFile(const Path& _path);
}