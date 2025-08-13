#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Texture.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "stb_image.h"
#include "TextureCube.h"

namespace Daydream
{
    Texture2D::Texture2D(const FilePath& _path)
    {
        path = _path;
    }

    Shared<Texture2D> Texture2D::Create(const FilePath& _path, const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateTexture2D(_path, _desc);
    }

    Shared<Texture2D> Texture2D::CreateEmpty(const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateEmptyTexture2D(_desc);
    }
}