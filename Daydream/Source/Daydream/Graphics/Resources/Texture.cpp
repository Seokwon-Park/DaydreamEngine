#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "TextureCube.h"

namespace Daydream
{
    Shared<Texture2D> Texture2D::Create(const FilePath& _path, const TextureDesc& _desc)
    {
        TextureDesc finalDesc = _desc;
        auto imageData = ImageLoader::LoadImageFile(_path);
        finalDesc.width = imageData.width;
        finalDesc.height = imageData.height;
        return Renderer::GetRenderDevice()->CreateTexture2D(imageData.data.data(), finalDesc);
    }

    Shared<Texture2D> Texture2D::CreateEmpty(const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateEmptyTexture2D(_desc);
    }
}