#include "SteinsPCH.h"
#include "VertexArray.h"

#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLVertexArray.h"
#include "Platform/RenderSystem/DirectX11/D3D11VertexArray.h"

namespace Steins {

	Shared<VertexArray> VertexArray::Create()
	{
		Shared<VertexArray> newVertexArray;
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:
		{
			newVertexArray = MakeShared<OpenGLVertexArray>();
			break;
		}
		case RendererAPIType::DirectX11:
		{
			newVertexArray = MakeShared<D3D11VertexArray>();
			break;
		}
		//case RendererAPIType::DirectX12:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::Vulkan:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::Metal:  return MakeShared<OpenGLVertexArray>();
		}

		if (newVertexArray != nullptr)
		{
			newVertexArray->device = Application::GetGraphicsDevice();
			return newVertexArray;
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}