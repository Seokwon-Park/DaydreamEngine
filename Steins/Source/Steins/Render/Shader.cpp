#include "SteinsPCH.h"
#include "Shader.h"

#include "Steins/Render/Renderer.h"

#include "Platform/RenderSystem/OpenGL/OpenGLShader.h"
#include "Platform/RenderSystem/DirectX11/D3D11Shader.h"


namespace Steins
{
	Shared<Shader> Shader::Create(const FilePath& _filepath, const ShaderType& _type)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:    STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLShader>(_filepath, _type);
		case RendererAPIType::DirectX11:  return MakeShared<D3D11Shader>(_filepath, _type);
			//case RendererAPIType::DirectX12:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Vulkan:  return new OpenGLVertexBuffer(_vertices, _size);
			//case RendererAPIType::Metal:  return new OpenGLVertexBuffer(_vertices, _size);
		}
		return nullptr;
	}

	Shared<Shader> Shader::Create(const std::string& _src, const ShaderType& _type)

	{
		switch (Renderer::GetAPI())
		{
		case RendererAPIType::None:		STEINS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return MakeShared<OpenGLShader>(_src, _type);
		case RendererAPIType::DirectX11:  return MakeShared<D3D11Shader>(_src, _type);
				//case RendererAPIType::DirectX12:  return new OpenGLVertexBuffer(_vertices, _size);
				//case RendererAPIType::Vulkan:  return new OpenGLVertexBuffer(_vertices, _size);
				//case RendererAPIType::Metal:  return new OpenGLVertexBuffer(_vertices, _size);
		}

		STEINS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
