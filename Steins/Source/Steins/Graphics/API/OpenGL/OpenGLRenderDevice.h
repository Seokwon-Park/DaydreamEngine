#pragma once
#include "Steins/Graphics/Core/RenderDevice.h"
#include "GLFW/glfw3.h"
//#include "glad/glad.h"

namespace Steins
{
	class OpenGLRenderDevice: public RenderDevice
	{
	public:
		OpenGLRenderDevice();
		~OpenGLRenderDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;


		virtual Shared<RenderContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride) override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) override;
		virtual Shared<Framebuffer> CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc & _desc) override;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<Swapchain> CreateSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path, const TextureDesc& _desc)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) override;

		std::string GetVersion() const { return version; }
	protected:

	private:

		std::string version = "#version 450";
	};
}
