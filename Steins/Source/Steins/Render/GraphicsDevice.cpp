#include "SteinsPCH.h"
#include "GraphicsDevice.h"

#include "Steins/Core/Window.h"
#include "RendererAPI.h"

#include "Platform/RenderSystem/DirectX11/D3D11GraphicsDevice.h"
#include "Platform/RenderSystem/OpenGL/OpenGLGraphicsDevice.h"

namespace Steins
{
    GraphicsDevice::GraphicsDevice(SteinsWindow* _window)
        :windowHandle(_window)
    {
    }
    GraphicsDevice* GraphicsDevice::Create(SteinsWindow* _window)
	{
        switch (RendererAPI::GetRendererAPI())
        {
        case RendererAPIType::None: return nullptr;
        case RendererAPIType::OpenGL: return new OpenGLGraphicsDevice(_window);
        case RendererAPIType::DirectX11: return new D3D11GraphicsDevice(_window);
        //case RendererAPIType::Vulkan:    return new D3D11GraphicsDevice(_window);
        default: return nullptr;
        }
	}
}
