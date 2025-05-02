#pragma once

#include "Steins/Render/GraphicsDevice.h"


namespace Steins
{
	class D3D11GraphicsDevice : public GraphicsDevice
	{
	public:
		D3D11GraphicsDevice();
		virtual ~D3D11GraphicsDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<VertexBuffer> CreateVertexBuffer(Float32* _vertices, UInt32 _size, const BufferLayout& _layout) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferSpecification _spec)override;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<VertexArray> CreateVertexArray() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(const void* _data, UInt32 _size) override { return nullptr; };


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

