#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"

namespace Daydream
{
	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext();

		virtual void BeginCommandList() override {};
		virtual void EndCommandList() override {};
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)  override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0)  override;

		virtual void BeginRendering(const RenderingInfo& _renderingInfo) override;
		virtual void EndRendering(const RenderingInfo& _renderingInfo) override;

		//virtual void BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer) override;
		//virtual void EndRenderPass(Shared<RenderPass> _renderPass) override;

		virtual void BindPipelineState(Shared<GraphicsPipelineState> _pipelineState)override;

		virtual void BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer) override;
		virtual void BindIndexBuffer(Shared<IndexBuffer> _indexBuffer) override;

		//virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture) override;
		//virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube) override;
		virtual void BindShaderResourceView(const String& _name, Shared<TextureView> _textureView, Shared<Sampler> _sampler) override;
		virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer) override;

		virtual void CopyBuffer(Shared<GPUBuffer> _src, Shared<GPUBuffer> _dst, UInt32 _copySize) override;
		virtual void CopyBufferToTexture(Shared<GPUBuffer> _src, Shared<GPUTexture> _dst) override;

		virtual void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst) override;
		virtual void CopyTextureToCubemapFace(Shared<Texture2D> _srcTexture2D, Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, UInt32 _mipLevel = 0)override;

		virtual void GenerateMips(Shared<Texture> _texture)override;

	private:
		UInt32 framebufferID;
	};
}

