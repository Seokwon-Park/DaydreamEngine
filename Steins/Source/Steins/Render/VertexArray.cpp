#include "SteinsPCH.h"
#include "VertexArray.h"
#include "GraphicsDevice.h"

#include "Steins/Core/Application.h"

#include "Steins/Render/Renderer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLVertexArray.h"
#include "Platform/RenderSystem/DirectX11/D3D11VertexArray.h"
#include "Platform/RenderSystem/DirectX12/D3D12VertexArray.h"

namespace Steins {

	Shared<VertexArray> VertexArray::Create()
	{
		Shared<VertexArray> newVertexArray;
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:	return MakeShared<OpenGLVertexArray>();
		case RendererAPIType::DirectX11:return  MakeShared<D3D11VertexArray>(Application::GetGraphicsDevice());
		case RendererAPIType::DirectX12:  return MakeShared<D3D12VertexArray>();
		//case RendererAPIType::Vulkan:  return MakeShared<OpenGLVertexArray>();
		//case RendererAPIType::Metal:  return MakeShared<OpenGLVertexArray>();
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}