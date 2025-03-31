#include "SteinsPCH.h"
#include "Buffer.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Enum/RendererAPI.h"

namespace Steins
{
	VertexBuffer* VertexBuffer::Create(float* _vertices, uint32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32 _vertices, uint32 _size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
