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

        stbi_set_flip_vertically_on_load(1);
        data = stbi_load(_path.ToString().c_str(), &width, &height, &channels, 0);
        STEINS_CORE_ASSERT(data, "Failed to load image!");
    }

    Shared<Texture2D> Texture2D::Create(const FilePath& _path)
    {
        return Application::GetGraphicsDevice()->CreateTexture2D(_path);
    }
}