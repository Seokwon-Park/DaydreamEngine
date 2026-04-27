#include "DaydreamPCH.h"
#include "Daydream/Graphics/Core/RenderDevice.h"

#include "Daydream/Core/Window.h"

#include "Daydream/Graphics/Core/Swapchain.h"

#include "Daydream/Graphics/API/OpenGL/OpenGLRenderDevice.h"
#include "Daydream/Graphics/API/DirectX11/D3D11RenderDevice.h"
#include "Daydream/Graphics/API/DirectX12/D3D12RenderDevice.h"
#include "Daydream/Graphics/API/Vulkan/VulkanRenderDevice.h"

#include "Daydream/Graphics/Resources/Buffer.h"

namespace Daydream
{
	Unique<RenderDevice> RenderDevice::Create(RendererAPIType _API)
	{
		switch (_API)
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return MakeUnique<OpenGLRenderDevice>();
		case RendererAPIType::DirectX11: return MakeUnique<D3D11RenderDevice>();
		case RendererAPIType::DirectX12: return MakeUnique<D3D12RenderDevice>();
		case RendererAPIType::Vulkan: return MakeUnique<VulkanRenderDevice>();
		default: return nullptr;
		}
	}

	//Shared<VertexBuffer> RenderDevice::CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	//{
	//	BufferDesc desc{};
	//	desc.bufferUsage = BufferUsage::Vertex;
	//	desc.memoryUsage = MemoryUsage::Dynamic;
	//	desc.size = _size;

	//	Shared<GPUBuffer> gpuBuffer = CreateGPUBuffer(desc);

	//	Shared<VertexBuffer> vertexBuffer = MakeShared<VertexBuffer>(gpuBuffer, _stride);
	//	if (_initialData != nullptr)
	//	{
	//		vertexBuffer->UpdateData(_initialData, _initialDataSize);
	//	}
	//	return vertexBuffer;
	//}

	//Shared<VertexBuffer> RenderDevice::CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData)
	//{
	//	BufferDesc desc{};
	//	desc.bufferUsage = BufferUsage::Vertex;
	//	desc.memoryUsage = MemoryUsage::Static;
	//	desc.size = _size;

	//	Shared<GPUBuffer> gpuBuffer = CreateGPUBuffer(desc);

	//	return MakeShared<VertexBuffer>(gpuBuffer, _stride);
	//}

	//Shared<IndexBuffer> RenderDevice::CreateIndexBuffer(const UInt32* _indices, UInt32 _count)
	//{
	//	BufferDesc desc{};
	//	desc.bufferUsage = BufferUsage::Index;
	//	desc.memoryUsage = MemoryUsage::Static;
	//	desc.size = sizeof(UInt32) * _count;

	//	Shared<GPUBuffer> gpuBuffer = CreateGPUBuffer(desc);

	//	return  MakeShared<IndexBuffer>(gpuBuffer, _count);
	//}

	//Shared<ConstantBuffer> RenderDevice::CreateConstantBuffer(UInt32 _size)
	//{
	//	BufferDesc desc{};
	//	desc.bufferUsage = BufferUsage::Constant;
	//	desc.memoryUsage = MemoryUsage::Dynamic;
	//	desc.size = _size;

	//	Shared<GPUBuffer> gpuBuffer = CreateGPUBuffer(desc);

	//	return MakeShared<ConstantBuffer>(gpuBuffer);
	//}
}
