#include "SteinsPCH.h"
#include "Texture.h"

#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"

namespace Steins
{
    Shared<Texture2D> Texture2D::Create(const FilePath& _path)
    {
        return Application::GetGraphicsDevice()->CreateTexture2D(_path);
    }
}