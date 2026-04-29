#include "DaydreamPCH.h"
#include "OpenGLShader.h"
#include "OpenGLBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureCube.h"
#include "OpenGLSampler.h"
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

		glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
		//GLint count;
		//glGetIntegerv(GL_NUM_EXTENSIONS, &count);

		//for (GLint i = 0; i < count; ++i)
		//{
		//	const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		//	if (!strcmp(extension, "GL_NVX_gpu_memory_info"))
		//		printf("%d: %s\n", i, extension);
		//}

		info.APIName = "OpenGL Info:";
		info.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		info.physicalDeviceInfo = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		info.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}
	void OpenGLRenderDevice::Shutdown()
	{
	}

	Unique<RenderContext> OpenGLRenderDevice::CreateContext()
	{
		return MakeUnique<OpenGLRenderContext>();
	}

	Shared<RenderCommandList> OpenGLRenderDevice::CreateRenderCommandList()
	{
		return MakeUnique<RenderCommandList>();
	}

	Shared<GPUBuffer> OpenGLRenderDevice::CreateGPUBuffer(const BufferDesc& _desc)
	{
		return MakeShared<OpenGLGPUBuffer>(_desc);
	}

	Shared<GPUTexture> Daydream::OpenGLRenderDevice::CreateGPUTexture(const TextureDesc & _desc)
	{
		return MakeShared<OpenGLGPUTexture>(_desc);
	}

	//Shared<VertexBuffer> OpenGLRenderDevice::CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	//{
	//	BufferDesc desc{};
	//	desc.bufferUsage = BufferUsage::Vertex;
	//	desc.memoryUsage = MemoryUsage::Dynamic;
	//	desc.size = _size;
	//	desc.initialData = _initialData;

	//	Shared<D3D11GPUBuffer> gpuBuffer = MakeShared<D3D11GPUBuffer>(this, desc);

	//	Shared<VertexBuffer> vertexBuffer = MakeShared<VertexBuffer>(gpuBuffer, _stride);
	//	if (_initialData != nullptr)
	//	{
	//		vertexBuffer->UpdateData(_initialData, _initialDataSize);
	//	}
	//	return vertexBuffer;
	//}

	//Shared<VertexBuffer> OpenGLRenderDevice::CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData)
	//{
	//	return MakeShared<OpenGLVertexBuffer>(_size, _stride, _initialData);
	//}

	//Shared<IndexBuffer> Daydream::OpenGLRenderDevice::CreateIndexBuffer(const UInt32* _indices, UInt32 _count)
	//{
	//	return MakeShared<OpenGLIndexBuffer>(_indices, _count);
	//}

	/*Shared<ConstantBuffer> OpenGLRenderDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<OpenGLConstantBuffer>(_size);
	}*/

	Shared<RenderPass> OpenGLRenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return MakeShared<OpenGLRenderPass>(_desc);
	}

	Shared<Framebuffer> OpenGLRenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return _renderPass->CreateFramebuffer(_desc);
	}

	Shared<PipelineState> OpenGLRenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<OpenGLPipelineState>(_desc);
	}

	Shared<Shader> OpenGLRenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<OpenGLShader>(_src, _type, _mode);
	}


	Shared<Swapchain> Daydream::OpenGLRenderDevice::CreateSwapchain(const DaydreamWindow& _window, const SwapchainDesc& _desc)
	{
		return MakeShared<OpenGLSwapchain>(_window, _desc);
	}

	/*Shared<Texture2D> OpenGLRenderDevice::CreateEmptyTexture2D(const TextureDesc& _desc)
	{
		return MakeShared<OpenGLTexture2D>(_desc);
	}

	Shared<Texture2D> OpenGLRenderDevice::CreateTexture2D(const void* _imageData, const TextureDesc& _desc)
	{
		return MakeShared<OpenGLTexture2D>(_desc, _imageData);
	}

	Shared<TextureCube> OpenGLRenderDevice::CreateTextureCube(Array<const void*>& _imagePixels, const TextureDesc& _desc)
	{
		return MakeShared<OpenGLTextureCube>(_desc, _imagePixels);
	}

	Shared<TextureCube> OpenGLRenderDevice::CreateTextureCube(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc)
	{
		Array<const void*> dummy;
		Shared<OpenGLTextureCube> textureCube = MakeShared<OpenGLTextureCube>(_desc, dummy);

		for (int i = 0; i < 6; i++)
		{
			CopyTextureToCubemapFace(textureCube.get(), i, _textures[i].get(), 0);
		}

		return textureCube;
	}

	Shared<TextureCube> OpenGLRenderDevice::CreateEmptyTextureCube(const TextureDesc& _desc)
	{
		Array<const void*> dummy;
		auto textureCube = MakeShared<OpenGLTextureCube>(_desc, dummy);

		return textureCube;
	}*/

	Shared<Sampler> OpenGLRenderDevice::CreateSampler(const SamplerDesc& _desc)
	{
		return  MakeUnique<OpenGLSampler>(_desc);
	}

	Unique<ImGuiRenderer> OpenGLRenderDevice::CreateImGuiRenderer()
	{
		return MakeUnique<OpenGLImGuiRenderer>(this);
	}

	//void OpenGLRenderDevice::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	//{
	//	OpenGLTexture2D* src = static_cast<OpenGLTexture2D*>(_src.get());
	//	OpenGLTexture2D* dst = static_cast<OpenGLTexture2D*>(_dst.get());
	//	glCopyImageSubData(
	//		src->GetTextureID(),          // 원본 텍스처 이름
	//		GL_TEXTURE_2D,       // 원본 텍스처 타입
	//		0,                   // 원본 밉맵 레벨
	//		0, 0, 0,             // 원본 오프셋 (x, y, z)
	//		dst->GetTextureID(),          // 대상 텍스처 이름
	//		GL_TEXTURE_2D,       // 대상 텍스처 타입
	//		0,                   // 대상 밉맵 레벨
	//		0, 0, 0,             // 대상 오프셋 (x, y, z)
	//		_src->GetWidth(),               // 복사할 너비
	//		_src->GetHeight(),              // 복사할 높이
	//		1                    // 복사할 깊이 (2D 텍스처는 1)
	//	);
	//}

	//void OpenGLRenderDevice::CopyTextureToCubemapFace(TextureCube* _dstCubemap, UInt32 _faceIndex, Texture2D* _srcTexture2D, UInt32 _mipLevel)
	//{
	//	OpenGLTexture2D* src = static_cast<OpenGLTexture2D*>(_srcTexture2D);
	//	OpenGLTextureCube* dst = static_cast<OpenGLTextureCube*>(_dstCubemap);
	//	glCopyImageSubData(
	//		src->GetTextureID(),      // 원본 텍스처 핸들
	//		GL_TEXTURE_2D,        // 원본 타겟 타입
	//		0,                    // 원본 밉 레벨
	//		0, 0, 0,              // 원본 좌표 (x, y, z)
	//		dst->GetTextureID(),         // 대상 텍스처 핸들
	//		GL_TEXTURE_CUBE_MAP,  // 대상 타겟 타입
	//		_mipLevel,             // 대상 밉 레벨
	//		0, 0, _faceIndex,      // 대상 좌표 (x, y, layer) - faceIndex가 레이어를 지정!
	//		src->GetWidth(), src->GetHeight(), 1      // 복사할 크기
	//	);
	//}
}
