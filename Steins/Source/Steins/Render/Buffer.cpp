#include "SteinsPCH.h"
#include "Buffer.h"
#include "GraphicsDevice.h"

#include "Steins/Render/Renderer.h"
#include "Steins/Render/GraphicsContext.h"

#include "Platform/RenderSystem/OpenGL/OpenGLBuffer.h"
#include "Platform/RenderSystem/DirectX11/D3D11Buffer.h"

namespace Steins
{
	/*Shared<VertexBuffer> VertexBuffer::Create(Float32* _vertices, UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateVertexBuffer(_vertices, _size);
	}*/
	Shared<VertexBuffer> VertexBuffer::Create(Float32* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		return Renderer::GetRenderDevice()->CreateVertexBuffer(_vertices, _size, _layout);
	}
	Shared<IndexBuffer> IndexBuffer::Create(UInt32* _indices, UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateIndexBuffer(_indices, _size);
	}
	Shared<ConstantBuffer> ConstantBuffer::Create(UInt32 _size)
	{
		return Renderer::GetRenderDevice()->CreateConstantBuffer(_size);
	}
}
