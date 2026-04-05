#pragma once

#include "Daydream/Graphics/Resources/Buffer.h"

#include "VulkanRenderDevice.h"

namespace Daydream
{
	class VulkanGPUBuffer : public GPUBuffer
	{
	public:
		VulkanGPUBuffer(VulkanRenderDevice* _device, const BufferDesc& _desc);
		virtual ~VulkanGPUBuffer();

		vk::Buffer GetVkBuffer() const { return buffer.get(); }

		static Shared<GPUBuffer> Create(UInt32 _size);
	protected:
		VulkanRenderDevice* device;
		vma::UniqueBuffer buffer;
		vma::UniqueAllocation bufferAllocation;
		vk::DeviceSize size;
		void* mappedData;

		// Inherited via GPUBuffer
		void* Map() override;
		void Unmap() override;
		void Update(const void* _data, UInt32 _size) override;
	};

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VulkanRenderDevice* _device, MemoryUsage _usage, UInt32 _size);
		virtual ~VulkanVertexBuffer() override;

		virtual void Bind() const;
		virtual void Unbind() const {};

		virtual void* GetNativeHandle() const override { return vertexBuffer.get(); };
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


		virtual void* GetNativeHandle() const override { return indexBuffer.get(); };
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

