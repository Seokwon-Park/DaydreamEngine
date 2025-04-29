#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererEnums.h"
#include "RendererAPI.h"
#include "Steins/ImGui/ImGuiRenderer.h"


namespace Steins
{
	struct SwapChainSpecification; 
	struct FramebufferSpecification;
	struct PipelineStateDesc;
	class SteinsWindow;
	class VertexBuffer;
	class IndexBuffer;
	class Framebuffer;
	class PipelineState;
	class Shader;
	class SwapChain;
	class Texture2D;
	class ImGuiRenderer;
	class VertexArray;
	class BufferLayout;

	class GraphicsDevice
	{
	public:
		virtual ~GraphicsDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;

		virtual Shared<VertexBuffer> CreateVertexBuffer(Float32* _vertices, UInt32 _size, const BufferLayout& _layout) = 0;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) = 0;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferSpecification _spec) = 0;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)= 0;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) = 0;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window) = 0;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path) = 0;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() = 0;
		virtual Shared<VertexArray> CreateVertexArray() = 0;

		inline RendererAPIType GetAPI() const { return API; }
		inline SwapChain* GetSwapChain(UInt32 _index)
		{
			STEINS_CORE_ASSERT(swapChains.size() > _index, "Not valid index!");
			return swapChains[_index];
		};
		inline void AddSwapChain(SwapChain* _swapChain) { swapChains.push_back(_swapChain); }

		static Unique<GraphicsDevice> Create(RendererAPIType _API);
	protected:
		std::vector<SwapChain*> swapChains;
		RendererAPIType API = RendererAPIType::None;
	};
}
