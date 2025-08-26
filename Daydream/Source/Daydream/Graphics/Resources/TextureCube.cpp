#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/TextureCube.h"

#include "Daydream/Graphics/Core/Renderer.h"


#include "Daydream/Graphics/Utility/ImageLoader.h"


namespace Daydream
{
	Shared<TextureCube> TextureCube::Create(const Array<FilePath>& _paths, const TextureDesc& _desc)
	{
        Array<const void*> imageDatas;
        TextureDesc finalDesc = _desc;
        for (auto path : _paths)
        {
            auto imageData = ImageLoader::LoadImageFile(path);

            if (imageData.data.data() != nullptr)
            {
                finalDesc.width = imageData.width;
                finalDesc.height = imageData.height;

                imageDatas.push_back(imageData.data.data());
            }
        }
        return Renderer::GetRenderDevice()->CreateTextureCube(imageDatas, finalDesc);
    }
}