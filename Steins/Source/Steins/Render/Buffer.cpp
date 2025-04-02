#include "SteinsPCH.h"
#include "Buffer.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Render/RendererAPI.h"

#include "Platform/RenderSystem/OpenGL/OpenGLVertexBuffer.h"

namespace Steins
{
	VertexBuffer* VertexBuffer::Create(float32* _vertices, uint32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexBuffer(_vertices, _size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32* _vertices, uint32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLIndexBuffer(_vertices, _size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
