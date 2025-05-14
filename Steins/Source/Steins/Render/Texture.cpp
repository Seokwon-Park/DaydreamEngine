#include "SteinsPCH.h"
#include "Texture.h"

#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"

#include "stb_image.h"

namespace Steins
{
    Texture2D::Texture2D(const FilePath& _path)
    {
        path = _path;
    }

    Shared<Texture2D> Texture2D::Create(const FilePath& _path)
    {
        return Application::GetGraphicsDevice()->CreateTexture2D(_path);
    }
}