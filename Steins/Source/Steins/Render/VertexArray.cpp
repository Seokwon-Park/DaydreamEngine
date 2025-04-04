#include "SteinsPCH.h"
#include "VertexArray.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLVertexArray.h"

namespace Steins {

	Shared<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLVertexArray>();
		case RendererAPIType::DirectX11:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::DirectX12:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::Vulkan:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::Metal:  return MakeShared<OpenGLVertexArray>();
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}