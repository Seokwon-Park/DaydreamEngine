#include "SteinsPCH.h"
#include "Shader.h"

#include "Steins/Render/Renderer.h"

#include "Platform/RenderSystem/OpenGL/OpenGLShader.h"


namespace Steins
{
	Shared<Shader> Shader::Create(const std::string& _vertexSrc, const std::string& _pixelSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLShader>(_vertexSrc, _pixelSrc);
		//case RendererAPIType::DirectX11:  return MakeShared<D3D11VertexBuffer>(_vertices, _size);
			//case RendererAPIType::DirectX12:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Vulkan:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Metal:  return new OpenGLVertexBuffer(_vertices, _size);
		}
		return nullptr;
	}
}
