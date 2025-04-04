#include "SteinsPCH.h"
#include "RenderCommand.h"

namespace Steins
{
	RendererAPI* RenderCommand::rendererAPI = nullptr;

	void RenderCommand::Init(GraphicsDevice* _device)
	{
		switch (RendererAPI::GetRendererAPI())
		{
		case RendererAPIType::None: rendererAPI = nullptr;
			break;
		case RendererAPIType::OpenGL: rendererAPI = new OpenGLRendererAPI();
			break;
		case RendererAPIType::DirectX11: rendererAPI = new D3D11RendererAPI();
			break;
			//case RendererAPIType::Vulkan:    return new D3D11GraphicsDevice(_window);
		default:
			break;;
		}
		rendererAPI->Init(_device);
	}
}
