#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/TextureCube.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"

namespace Daydream
{
	Shared<TextureCube> TextureCube::Create(const Array<Path>& _paths, const TextureDesc& _desc)
	{
        Array<const void*> imageDatas;
        Array<ImageLoader::ImageData> temp;
        TextureDesc finalDesc = _desc;
        for (auto path : _paths)
        {
            ImageLoader::ImageData imageData = ImageLoader::LoadImageFile(path);

            if (std::get<Array<UInt8>>(imageData.data).data() != nullptr)
            {
                finalDesc.width = imageData.width;
                finalDesc.height = imageData.height;

                temp.push_back(imageData);
            }           
        }
        for (int i = 0; i < temp.size(); i++)
        {
            imageDatas.push_back(std::get<Array<UInt8>>(temp[i].data).data());
        }
        return Renderer::GetRenderDevice()->CreateTextureCube(imageDatas, finalDesc);
    }
}