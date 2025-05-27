#include "SteinsPCH.h"
#include "OpenGLGraphicsDevice.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLSwapchain.h"
#include "OpenGLGraphicsContext.h"
#include "OpenGLPipelineState.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"
#include "OpenGLImGuiRenderer.h"

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


	Shared<GraphicsContext> OpenGLGraphicsDevice::CreateContext()
	{
		return Shared<GraphicsContext>();
	}

	Shared<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(Float32* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		return MakeShared<OpenGLVertexBuffer>(_vertices, _size, _layout);
	}

	Shared<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(UInt32* _indices, UInt32 _count)
	{
		return MakeShared<OpenGLIndexBuffer>(_indices, _count);
	}

	Shared<Framebuffer> OpenGLGraphicsDevice::CreateFramebuffer(FramebufferDesc _spec)
	{
		return Shared<Framebuffer>();
	}

	Shared<PipelineState> OpenGLGraphicsDevice::CreatePipelineState(PipelineStateDesc _desc)
	{
		return MakeShared<OpenGLPipelineState>(_desc);
	}

	Shared<Shader> OpenGLGraphicsDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<OpenGLShader>(_src, _type, _mode);
	}


	Shared<SwapChain> OpenGLGraphicsDevice::CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		return MakeShared<OpenGLSwapChain>(_desc, _window);
	}

	Shared<Texture2D> OpenGLGraphicsDevice::CreateTexture2D(const FilePath& _path)
	{
		return MakeShared<OpenGLTexture2D>(_path);
	}

	Unique<ImGuiRenderer> OpenGLGraphicsDevice::CreateImGuiRenderer()
	{
		return MakeUnique<OpenGLImGuiRenderer>(this);
	}

	Shared<VertexArray> OpenGLGraphicsDevice::CreateVertexArray()
	{
		return MakeShared<OpenGLVertexArray>();
	}

	Shared<ConstantBuffer> OpenGLGraphicsDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<OpenGLConstantBuffer>(_size);
	}

}
