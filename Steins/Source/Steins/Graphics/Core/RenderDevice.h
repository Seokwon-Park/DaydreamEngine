#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererEnums.h"
#include "Steins/ImGui/ImGuiRenderer.h"
#include "Steins/Graphics/Core/RenderContext.h"

namespace Steins
{
	struct SwapchainDesc; 
	struct RenderPassDesc;
	struct FramebufferDesc;
	struct PipelineStateDesc;
	struct TextureDesc;
	struct ShaderResourceViewDesc;
	class SteinsWindow;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class RenderPass;
	class Framebuffer;
	class PipelineState;
	class Shader;
	class Swapchain;
	class Texture;
	class Texture2D;
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

		virtual Shared<RenderContext> CreateContext() = 0;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride) = 0;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride) = 0;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) = 0;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) = 0;
		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _spec) = 0;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc)= 0;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) = 0;
		virtual Shared<Swapchain> CreateSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc) = 0;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path, const TextureDesc& _desc) = 0;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() = 0;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) = 0;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) = 0;
		
		virtual void* GetImGuiTextureID(Shared<Texture2D> _texture) { return nullptr; };
		
		void CreateSwapchainForWnd(SteinsWindow* _window);

		inline RendererAPIType GetAPI() const { return API; }
		inline Swapchain* GetSwapchain(UInt32 _index)
		{
			STEINS_CORE_ASSERT(swapChains.size() > _index, "Not valid index!");
			return swapChains[_index];
		};
		inline void AddSwapchain(Swapchain* _swapChain) { swapChains.push_back(_swapChain); }

		static Unique<RenderDevice> Create(RendererAPIType _API);
	protected:
		Array<Swapchain*> swapChains;
		RendererAPIType API = RendererAPIType::None;
	};
}
