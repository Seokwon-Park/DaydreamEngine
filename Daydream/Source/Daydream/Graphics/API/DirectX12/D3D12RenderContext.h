#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"

#include "D3D12RenderDevice.h"
#include "D3D12Framebuffer.h"
#include "D3D12RenderCommandList.h"

namespace Daydream
{
	class D3D12RenderContext : public RenderContext
	{
	public:
		D3D12RenderContext(D3D12RenderDevice* _device);
		virtual ~D3D12RenderContext() {};

		virtual void BeginCommandList() override {};
		virtual void EndCommandList() override {};
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)  override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0)  override;

		virtual void BeginRendering(const RenderingInfo& _renderingInfo) override;
		virtual void EndRendering(const RenderingInfo& _renderingInfo) override;

		virtual void BindPipelineState(Shared<GraphicsPipelineState> _pipelineState)override;

		virtual void BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer) override;
		virtual void BindIndexBuffer(Shared<IndexBuffer> _indexBuffer) override;

		//virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture) override;
		//virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube) override;
		virtual void BindShaderResourceView(const String& _name, Shared<TextureView> _textureView, Shared<Sampler> _sampler) override;
		virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer) override;

		virtual void CopyBuffer(Shared<GPUBuffer> _src, Shared<GPUBuffer> _dst, UInt32 _copySize) override;
		virtual void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst) override;
		virtual void CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0)override;


		//TODO : need to fix
		virtual void GenerateMips(Shared<Texture> _texture) override;

		virtual void TransitionTextureState(Shared<GPUTexture> _texture,
			ResourceState _beforeState,
			ResourceState _afterState,
			UInt32 _mipLevel = 0,
			UInt32 _mipCount = 1) override;

		virtual void TransitionBufferState(
			Shared<GPUBuffer> _buffer,
			ResourceState _beforeState,
			ResourceState _afterState
		) override;

		virtual void SetActiveCommandList(Shared<RenderCommandList> _commandList) override;
	private:
		inline ID3D12GraphicsCommandList* GetD3D12ActiveCommandList() const { return activeD3D12CommandList.Get(); }

		D3D12RenderDevice* device;

		ComPtr<ID3D12GraphicsCommandList> activeD3D12CommandList;
	};
}

