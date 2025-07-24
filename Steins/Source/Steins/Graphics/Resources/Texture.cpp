#include "SteinsPCH.h"
#include "Steins/Graphics/Resources/Texture.h"

#include "Steins/Core/Application.h"

#include "Steins/Graphics/Core/Renderer.h"

#include "stb_image.h"

namespace Steins
{
    Texture2D::Texture2D(const FilePath& _path)
    {
        path = _path;
    }

    Shared<Texture2D> Texture2D::Create(const FilePath& _path, const TextureDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateTexture2D(_path, _desc);
    }
}