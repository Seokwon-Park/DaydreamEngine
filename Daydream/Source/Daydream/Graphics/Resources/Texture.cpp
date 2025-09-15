#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"


namespace Daydream
{
    Shared<Texture2D> Texture2D::Create(const void* _data, const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateTexture2D(_data, _desc);
    }

    Shared<Texture2D> Texture2D::CreateFromFile(const Path& _path, const TextureDesc& _desc)
    {
        String ext = _path.extension().string();
        DAYDREAM_CORE_INFO("{}", ext);
        if (_path.extension().string() == ".png" || _path.extension().string() == ".jpg")
        {
            auto imageData = ImageLoader::LoadImageFile(_path);
            if (std::get<Array<UInt8>>(imageData.data).data() != nullptr)
            {
                TextureDesc finalDesc = _desc;
                finalDesc.width = imageData.width;
                finalDesc.height = imageData.height;

                return Renderer::GetRenderDevice()->CreateTexture2D(std::get<Array<UInt8>>(imageData.data).data(), finalDesc);
            }
        }
        else if (_path.extension().string() == ".exr" || _path.extension().string() == ".hdr")
        {
            auto imageData = ImageLoader::LoadHDRIFile(_path);
            if (imageData.data.data() != nullptr)
            {
                TextureDesc finalDesc = _desc;
                finalDesc.width = imageData.width;
                finalDesc.height = imageData.height;
                finalDesc.format = RenderFormat::R32G32B32A32_FLOAT;
                return Renderer::GetRenderDevice()->CreateTexture2D(imageData.data.data(), finalDesc);
            }
        }
        else
        {
            return Renderer::GetRenderDevice()->CreateTexture2D(nullptr, _desc);
        }
    }

    Shared<Texture2D> Texture2D::CreateEmpty(const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateEmptyTexture2D(_desc);
    }
}