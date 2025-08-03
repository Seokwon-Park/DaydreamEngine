#include "DaydreamPCH.h"
#include "OpenGLShader.h"
#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLSwapchain.h"
#include "OpenGLRenderPass.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLRenderDevice.h"
#include "OpenGLRenderContext.h"
#include "OpenGLPipelineState.h"
#include "OpenGLImGuiRenderer.h"

#include "glad/glad.h"
#include "Daydream/Core/Window.h"

namespace Daydream
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
		DAYDREAM_CORE_ASSERT(status, "Failed to initialize Glad!");

		//GLint count;
		//glGetIntegerv(GL_NUM_EXTENSIONS, &count);

		//for (GLint i = 0; i < count; ++i)
		//{
		//	const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		//	if (!strcmp(extension, "GL_NVX_gpu_memory_info"))
		//		printf("%d: %s\n", i, extension);
		//}
		
		DAYDREAM_CORE_INFO("OpenGL Info:");
		DAYDREAM_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		DAYDREAM_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		DAYDREAM_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
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

	Shared<VertexBuffer> Daydream::OpenGLRenderDevice::CreateStaticVertexBuffer(void* _vertices, UInt32 _size, UInt32 _stride)
	{
		return MakeShared<OpenGLVertexBuffer>(_vertices, _size, _stride);
	}

	Shared<IndexBuffer> OpenGLRenderDevice::CreateIndexBuffer(UInt32* _indices, UInt32 _count)
	{
		return MakeShared<OpenGLIndexBuffer>(_indices, _count);
	}

	Shared<RenderPass> OpenGLRenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return MakeShared<OpenGLRenderPass>(_desc);
	}

	Shared<Framebuffer> Daydream::OpenGLRenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc &_desc)
	{
		return _renderPass->CreateFramebuffer(_desc);
	}

	Shared<PipelineState> Daydream::OpenGLRenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<OpenGLPipelineState>(_desc);
	}

	Shared<Shader> OpenGLRenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<OpenGLShader>(_src, _type, _mode);
	}


	Shared<Swapchain> OpenGLRenderDevice::CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		return MakeShared<OpenGLSwapchain>(_window, _desc);
	}

	Shared<Texture2D> OpenGLRenderDevice::CreateEmptyTexture2D(const TextureDesc& _desc)
	{
		return MakeShared<OpenGLTexture2D>(_desc);
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
