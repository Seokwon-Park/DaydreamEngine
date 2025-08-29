#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"


namespace Daydream
{
    Shared<Texture2D> Texture2D::Create(const Path& _path, const TextureDesc& _desc)
    {
        auto imageData = ImageLoader::LoadImageFile(_path);
        if (imageData.data.data() != nullptr)
        {
            TextureDesc finalDesc = _desc;
            finalDesc.width = imageData.width;
            finalDesc.height = imageData.height;

            DAYDREAM_CORE_TRACE("CREATE TEXTURE {}", _path.GetFileName());

            return Renderer::GetRenderDevice()->CreateTexture2D(imageData.data.data(), finalDesc);
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