#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"

#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanRenderDevice* _device, BufferUsage _usage, UInt32 _size);
		virtual ~VulkanVertexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const {};
		virtual void SetData(const void* _data, UInt32 _dataSize) override;

		const vk::Buffer GetVkBuffer() const { return vertexBuffer.get(); }
	private:
		VulkanRenderDevice* device;
		vma::UniqueBuffer vertexBuffer;
		vma::UniqueAllocation vertexBufferAllocation;
		vk::DeviceSize size;
		void* mappedData;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(VulkanRenderDevice* _device, UInt32 _indexCount);
		virtual ~VulkanIndexBuffer() override {};

		virtual void Bind() const;
		virtual void Unbind() const {};

		vk::Buffer GetVkBuffer() { return indexBuffer.get(); }
	private:
		VulkanRenderDevice* device;
		vma::UniqueBuffer indexBuffer;
		vma::UniqueAllocation indexBufferAllocation;
	};

	class VulkanConstantBuffer : public ConstantBuffer
	{
	public:
		VulkanConstantBuffer(VulkanRenderDevice* _device, UInt32 _size);
		virtual ~VulkanConstantBuffer() override;

		//virtual void Bind(UInt32 _slot) const override;
		virtual void Update(const void* _data, UInt32 _size);

		virtual void* GetNativeHandle() override { return constantBuffer.get(); }
	private:
		VulkanRenderDevice* device;
		vma::UniqueBuffer constantBuffer;
		vma::UniqueAllocation constantBufferAllocation;
		void* mappedData;
	};
}

