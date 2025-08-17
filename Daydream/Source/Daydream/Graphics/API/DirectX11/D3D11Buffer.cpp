#include "DaydreamPCH.h"
#include "D3D11Buffer.h"

#include "D3D11RenderDevice.h"

namespace Daydream
{
	D3D11VertexBuffer::D3D11VertexBuffer(D3D11RenderDevice* _device, BufferUsage _usage, UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		device = _device;
		stride = _stride;
		usage = _usage;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = _size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		
		HRESULT hr;
		if (usage == BufferUsage::Static)
		{
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA initialData;
			ZeroMemory(&initialData, sizeof(initialData));
			initialData.pSysMem = _initialData;
			initialData.SysMemPitch = 0;
			initialData.SysMemSlicePitch = 0;

			hr = device->GetDevice()->CreateBuffer(&bufferDesc, &initialData, vertexBuffer.GetAddressOf());
		}
		else
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			hr = device->GetDevice()->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
		}
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create VertexBuffer");
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
	}
	void D3D11VertexBuffer::Bind() const
	{
		UINT offset = 0;
		device->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
	void D3D11VertexBuffer::Unbind() const
	{
	}
	void Daydream::D3D11VertexBuffer::SetData(const void* _data, UInt32 _dataSize)
	{
		if (usage == BufferUsage::Static)
		{
			DAYDREAM_CORE_ASSERT(false, "Cannot call SetData on a static buffer after creation!");
			return;
		}
		D3D11_MAPPED_SUBRESOURCE sub;
		device->GetContext()->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, _data, _dataSize);
		device->GetContext()->Unmap(vertexBuffer.Get(), 0);
	}
	D3D11IndexBuffer::D3D11IndexBuffer(D3D11RenderDevice* _device, UInt32* _indices, UInt32 _indexCount)
	{
		device = _device;
		indexCount = _indexCount;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = sizeof(UInt32) * indexCount;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = _indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		device->GetDevice()->CreateBuffer(&bufferDesc, &indexData, indexBuffer.GetAddressOf());
	}


	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
	}
	void D3D11IndexBuffer::Bind() const
	{
		device->GetContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	void D3D11IndexBuffer::Unbind() const
	{
	}

	D3D11ConstantBuffer::D3D11ConstantBuffer(D3D11RenderDevice* _device, UInt32 _size)
	{
		device = _device;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = _size;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		device->GetDevice()->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	}
	D3D11ConstantBuffer::~D3D11ConstantBuffer()
	{
	}

	void D3D11ConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		D3D11_MAPPED_SUBRESOURCE data;
		device->GetContext()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		memcpy(data.pData, _data, _size);
		device->GetContext()->Unmap(constantBuffer.Get(), 0);
	}
}