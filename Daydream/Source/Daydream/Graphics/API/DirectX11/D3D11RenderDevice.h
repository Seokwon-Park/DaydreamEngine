#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"


namespace Daydream
{
	class D3D11RenderDevice : public RenderDevice
	{
	public:
		D3D11RenderDevice();
		virtual ~D3D11RenderDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<RenderContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr) override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) override;
		virtual Shared<Framebuffer> CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc & _desc) override;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<Swapchain> CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)override;
		virtual Shared<Texture2D> CreateTexture2D(const void* _imageData, const TextureDesc& _desc)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) override;

		ID3D11Device* GetDevice() { return device.Get(); }
		ID3D11DeviceContext* GetContext() { return deviceContext.Get(); }
		IDXGIFactory7* GetFactory() { return dxgiFactory.Get(); }
	private:
		bool debugLayerEnabled = true;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;

		ComPtr<ID3D11Debug> debugLayer = nullptr;
		ComPtr<ID3D11Device> device = nullptr;
		ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	};
}

