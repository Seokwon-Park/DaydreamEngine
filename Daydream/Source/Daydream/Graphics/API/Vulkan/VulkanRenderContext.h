#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"
#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanRenderContext : public RenderContext
	{
	public:
		VulkanRenderContext(VulkanRenderDevice* _device, UInt32 _framesInFlight);
		virtual ~VulkanRenderContext();

		virtual void BeginCommandList() override;
		virtual void EndCommandList() override;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) override;
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

		virtual void BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer) override;
		virtual void EndRenderPass(Shared<RenderPass> _renderPass) override;

		virtual void BindPipelineState(Shared<PipelineState> _pipelineState) override;

		virtual void BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer) override;
		virtual void BindIndexBuffer(Shared<IndexBuffer> _indexBuffer) override;

		virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture) override;
		virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube) override;
		virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer) override;

		virtual void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst) override;
		virtual void CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0) override;

		virtual void GenerateMips(Shared<Texture> _texture) override;
	private:
		vk::CommandBuffer GetActiveCommandBuffer();
		
		VulkanRenderDevice* device;

		Array<vk::UniqueFence> waitFences;
		Array<vk::UniqueCommandBuffer> commandBuffers;
		UInt32 commandBufferIndex;
	};
}