#include "SteinsPCH.h"
#include "OpenGLRenderDevice.h"
#include "OpenGLRenderContext.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLSwapchain.h"
#include "OpenGLRenderContext.h"
#include "OpenGLPipelineState.h"
#include "OpenGLTexture.h"
#include "OpenGLImGuiRenderer.h"

#include "Steins/Core/Window.h"
#include "glad/glad.h"

namespace Steins
{
	OpenGLRenderDevice::OpenGLRenderDevice()
	{
		API = RendererAPIType::OpenGL;
	}

	OpenGLRenderDevice::~OpenGLRenderDevice()
	{

	}
	void OpenGLRenderDevice::Init()
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
	void OpenGLRenderDevice::Shutdown()
	{
	}
	void OpenGLRenderDevice::Render()
	{
	}


	Shared<RenderContext> OpenGLRenderDevice::CreateContext()
	{
		return MakeShared<OpenGLGraphicsContext>();
	}

	Shared<VertexBuffer> OpenGLRenderDevice::CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride)
	{
		return MakeShared<OpenGLVertexBuffer>(_bufferSize, _stride);
	}

	Shared<VertexBuffer> OpenGLRenderDevice::CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride)
	{
		return MakeShared<OpenGLVertexBuffer>(_vertices, _size, _stride);
	}

	Shared<IndexBuffer> OpenGLRenderDevice::CreateIndexBuffer(UInt32* _indices, UInt32 _count)
	{
		return MakeShared<OpenGLIndexBuffer>(_indices, _count);
	}

	Shared<RenderPass> OpenGLRenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return Shared<RenderPass>();
	}

	Shared<Framebuffer> Steins::OpenGLRenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc & _desc)
	{
		return Shared<Framebuffer>();
	}

	Shared<PipelineState> Steins::OpenGLRenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<OpenGLPipelineState>(_desc);
	}

	Shared<Shader> OpenGLRenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<OpenGLShader>(_src, _type, _mode);
	}


	Shared<Swapchain> OpenGLRenderDevice::CreateSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc)
	{
		return MakeShared<OpenGLSwapchain>(_window, _desc);
	}

	Shared<Texture2D> OpenGLRenderDevice::CreateTexture2D(const FilePath& _path, const TextureDesc& _desc)
	{
		return MakeShared<OpenGLTexture2D>(_path, _desc);
	}

	Unique<ImGuiRenderer> OpenGLRenderDevice::CreateImGuiRenderer()
	{
		return MakeUnique<OpenGLImGuiRenderer>(this);
	}

	Shared<ConstantBuffer> OpenGLRenderDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<OpenGLConstantBuffer>(_size);
	}

	Shared<Material> OpenGLRenderDevice::CreateMaterial(Shared<PipelineState> _pipeline)
	{
		return _pipeline->CreateMaterial();
	}

}
