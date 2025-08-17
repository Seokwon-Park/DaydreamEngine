#include "DaydreamPCH.h"
#include "VulkanBuffer.h"

namespace Daydream
{
	VulkanVertexBuffer::VulkanVertexBuffer(VulkanRenderDevice* _device, BufferUsage _usage, UInt32 _size)
	{
		device = _device;
		size = _size;
		usage = _usage;

		if (usage == BufferUsage::Static)
		{
			std::tie(vertexBuffer, vertexBufferAllocation) = device->CreateBuffer(_size,
				vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
				vma::MemoryUsage::eGpuOnly, {});
		}
		else if (usage == BufferUsage::Dynamic)
		{
			std::tie(vertexBuffer, vertexBufferAllocation) = device->CreateBuffer(_size,
				vk::BufferUsageFlagBits::eVertexBuffer,
				vma::MemoryUsage::eCpuToGpu, vma::AllocationCreateFlagBits::eMapped | vma::AllocationCreateFlagBits::eHostAccessSequentialWrite);

			vma::AllocationInfo allocationInfo;
			device->GetAllocator().getAllocationInfo(vertexBufferAllocation.get(), &allocationInfo);

			mappedData = allocationInfo.pMappedData;
		}
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}

	void VulkanVertexBuffer::Bind() const
	{
		vk::DeviceSize offset = 0;
		device->GetCommandBuffer().bindVertexBuffers(0, 1, &vertexBuffer.get(), &offset);
	}

	void VulkanVertexBuffer::SetData(const void* _data, UInt32 _dataSize)
	{
		if (usage == BufferUsage::Static)
		{
			DAYDREAM_CORE_ASSERT(false, "Cannot call SetData on a static buffer after creation!");
		}
		//void* mappedMemory = device->GetDevice().mapMemory(vertexBufferMemory.get(), 0, _dataSize);
		//memcpy(mappedMemory, _data, _dataSize);
		//device->GetDevice().unmapMemory(vertexBufferMemory.get());
		memcpy(mappedData, _data, _dataSize);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanIndexBuffer::VulkanIndexBuffer(VulkanRenderDevice* _device, UInt32 _indexCount)
	{
		device = _device;
		indexCount = _indexCount;

		UInt32 bufferSize = sizeof(UInt32) * _indexCount;
		std::tie(indexBuffer, indexBufferAllocation) = device->CreateBuffer(bufferSize,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
			vma::MemoryUsage::eGpuOnly, {});
	}

	void VulkanIndexBuffer::Bind() const
	{
		device->GetCommandBuffer().bindIndexBuffer(*indexBuffer, 0, vk::IndexType::eUint32);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VulkanConstantBuffer::VulkanConstantBuffer(VulkanRenderDevice* _device, UInt32 _size)
	{
		device = _device;
		size = _size;
		//device->CreateBuffer(_size, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		std::tie(constantBuffer, constantBufferAllocation) = device->CreateBuffer(_size,
			vk::BufferUsageFlagBits::eUniformBuffer,
			vma::MemoryUsage::eCpuToGpu, vma::AllocationCreateFlagBits::eMapped | vma::AllocationCreateFlagBits::eHostAccessSequentialWrite);

		vma::AllocationInfo allocationInfo;
		device->GetAllocator().getAllocationInfo(constantBufferAllocation.get(), &allocationInfo);

		mappedData = allocationInfo.pMappedData;
	}

	VulkanConstantBuffer::~VulkanConstantBuffer()
	{

	}

	void VulkanConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		memcpy(mappedData, _data, _size);
	}

}
