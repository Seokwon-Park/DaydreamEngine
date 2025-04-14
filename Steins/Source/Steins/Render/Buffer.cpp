#include "SteinsPCH.h"
#include "Buffer.h"
#include "GraphicsDevice.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Render/RendererAPI.h"

#include "Steins/Core/Application.h"

#include "Platform/RenderSystem/OpenGL/OpenGLBuffer.h"
#include "Platform/RenderSystem/DirectX11/D3D11Buffer.h"

namespace Steins
{
	Shared<VertexBuffer> VertexBuffer::Create(Float32* _vertices, UInt32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLVertexBuffer>(_vertices, _size);
		case RendererAPIType::DirectX11:  return MakeShared<D3D11VertexBuffer>(Application::GetGraphicsDevice(), _vertices, _size);
			//case RendererAPIType::DirectX12:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Vulkan:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Metal:  return new OpenGLVertexBuffer(_vertices, _size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Shared<IndexBuffer> IndexBuffer::Create(UInt32* _vertices, UInt32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLIndexBuffer>(_vertices, _size);
		case RendererAPIType::DirectX11:  return MakeShared<D3D11IndexBuffer>(Application::GetGraphicsDevice(), _vertices, _size);
			//case RendererAPIType::DirectX12:  return new OpenGLIndexBuffer(_vertices, _size);
			//case RendererAPIType::Vulkan:  return new OpenGLIndexBuffer(_vertices, _size);
			//case RendererAPIType::Metal:  return new OpenGLIndexBuffer(_vertices, _size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
