#include "SteinsPCH.h"
#include "OpenGLGraphicsDevice.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLPipelineState.h"

#include "Steins/Core/Window.h"
#include "glad/glad.h"

namespace Steins
{
	OpenGLGraphicsDevice::OpenGLGraphicsDevice()
	{
		API = RendererAPIType::OpenGL;
	}

	OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
	{

	}
	void OpenGLGraphicsDevice::Init()
	{
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		STEINS_CORE_ASSERT(status, "Failed to initialize Glad!");

		//GLint count;
		//glGetIntegerv(GL_NUM_EXTENSIONS, &count);

		//for (GLint i = 0; i < count; ++i)
		//{
		//	const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		//	if (!strcmp(extension, "GL_NVX_gpu_memory_info"))
		//		printf("%d: %s\n", i, extension);
		//}
		
		STEINS_CORE_INFO("OpenGL Info:");
		STEINS_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		STEINS_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		STEINS_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}
	void OpenGLGraphicsDevice::Shutdown()
	{
	}
	void OpenGLGraphicsDevice::Render()
	{
	}

	Shared<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(Float32* _vertices, UInt32 _size)
	{
		return Shared<VertexBuffer>();
	}

	Shared<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(UInt32* _indices, UInt32 _count)
	{
		return Shared<IndexBuffer>();
	}

	Shared<Framebuffer> OpenGLGraphicsDevice::CreateFramebuffer(FramebufferSpecification _spec)
	{
		return Shared<Framebuffer>();
	}

	Shared<PipelineState> OpenGLGraphicsDevice::CreatePipelineState(PipelineStateDesc _desc)
	{
		return Shared<PipelineState>();
	}

	Shared<Shader> OpenGLGraphicsDevice::CreateShader(const FilePath& _filepath, const ShaderType& _type)
	{
		return Shared<Shader>();
	}

	Shared<Shader> OpenGLGraphicsDevice::CreateShader(const std::string& _src, const ShaderType& _type)
	{
		return Shared<Shader>();
	}

	Shared<SwapChain> OpenGLGraphicsDevice::CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		return Shared<SwapChain>();
	}

	Shared<Texture2D> OpenGLGraphicsDevice::CreateTexture2D(const FilePath& _path)
	{
		return Shared<Texture2D>();
	}

}
