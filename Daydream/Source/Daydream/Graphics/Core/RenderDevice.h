#pragma once

#include "Base/MathTypes.h"
#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/ImGui/ImGuiRenderer.h"
#include "Daydream/Graphics/Core/RenderContext.h"

namespace Daydream
{
	struct SwapchainDesc;
	struct RenderPassDesc;
	struct FramebufferDesc;
	struct PipelineStateDesc;
	struct TextureDesc;
	struct SamplerDesc;
	struct ShaderResourceViewDesc;
	class DaydreamWindow;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class RenderPass;
	class Framebuffer;
	class PipelineState;
	class Shader;
	class Swapchain;
	class Texture2D;
	class TextureCube;
	class Sampler;
	class ImGuiRenderer;
	class VertexArray;
	class BufferLayout;
	class Material;

	class RenderDevice
	{
	public:
		virtual ~RenderDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;

		virtual Unique<RenderContext> CreateContext() = 0;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr) = 0;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData) = 0;
		virtual Shared<IndexBuffer> CreateIndexBuffer(const UInt32* _indices, UInt32 _count) = 0;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) = 0;
		virtual Shared<Framebuffer> CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc) = 0;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc) = 0;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) = 0;
		virtual Shared<Swapchain> CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc) = 0;
		virtual Shared<Texture2D> CreateEmptyTexture2D(const TextureDesc& _desc) = 0; // TODO : 이 기능이 필요한지 아닌지 모름
		virtual Shared<Texture2D> CreateTexture2D(const void* _imageData, const TextureDesc& _desc)  = 0;
		virtual Shared<TextureCube> CreateTextureCube(Array<const void*>& _imagePixels, const TextureDesc& _desc) = 0;
		virtual Shared<TextureCube> CreateTextureCube(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc) { return nullptr; }
		virtual Shared<TextureCube> CreateEmptyTextureCube(const TextureDesc& _desc) { return nullptr; }
		virtual Shared<Sampler> CreateSampler(const SamplerDesc& _desc) = 0;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() = 0;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) = 0;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) = 0;

		virtual void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst) = 0;
		virtual void CopyTextureToCubemapFace(TextureCube* _dstCubemap, UInt32 _faceIndex, Texture2D* _srcTexture2D, UInt32 _mipLevel = 0) {}

		void CreateSwapchainForWindow(DaydreamWindow* _window);
		void AddFramebufferResizeRequest(Framebuffer* _framebuffer, Vector2 _size) { framebufferResizeInfo.push_back({ _framebuffer, _size }); };
		Array<Pair<Framebuffer*, Vector2>>& GetFramebufferResizeRequest() { return framebufferResizeInfo; }

		inline RendererAPIType GetAPI() const { return API; }

		static Unique<RenderDevice> Create(RendererAPIType _API);
	protected:
		RendererAPIType API = RendererAPIType::None;
		Array<Pair<Framebuffer*, Vector2>> framebufferResizeInfo;

	};
}
