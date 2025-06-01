#include "SteinsPCH.h"
#include "D3D11Buffer.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11VertexBuffer::D3D11VertexBuffer(D3D11GraphicsDevice* _device, UInt32 _bufferSize, UInt32 _stride)
	{
		device = _device;
		stride = _stride;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = _bufferSize;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		device->GetDevice()->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
	}

	D3D11VertexBuffer::D3D11VertexBuffer(D3D11GraphicsDevice* _device, void* _vertices, UInt32 _size, UInt32 _stride)
	{
		device = _device;
		stride = _stride;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = _size;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		device->GetDevice()->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());
	}


	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
	}
	void D3D11VertexBuffer::Bind() const
	{
		UINT offset = 0;
		device->GetContext()->IASetVertexBuffers(slot, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
	void D3D11VertexBuffer::Unbind() const
	{
	}
	void D3D11VertexBuffer::SetData(void* _data, UInt32 _dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE sub;
		device->GetContext()->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, _data, _dataSize);
		device->GetContext()->Unmap(vertexBuffer.Get(), 0);
	}
	D3D11IndexBuffer::D3D11IndexBuffer(D3D11GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount)
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

	D3D11ConstantBuffer::D3D11ConstantBuffer(D3D11GraphicsDevice* _device, UInt32 _size)
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
	void D3D11ConstantBuffer::Bind(UInt32 _slot, ShaderStage _flags) const
	{
		if (_flags & ShaderStage::SteinsVertexBit)
		{
			device->GetContext()->VSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
		if (_flags & ShaderStage::SteinsHullBit)
		{
			device->GetContext()->HSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
		if (_flags & ShaderStage::SteinsDomainBit)
		{
			device->GetContext()->DSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
		if (_flags & ShaderStage::SteinsGeometryBit)
		{
			device->GetContext()->GSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
		if (_flags & ShaderStage::SteinsPixelBit)
		{
			device->GetContext()->PSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
		if (_flags & ShaderStage::SteinsComputeBit)
		{
			device->GetContext()->CSSetConstantBuffers(_slot, 1, constantBuffer.GetAddressOf());
		}
	}
	void D3D11ConstantBuffer::Update(const void* _data, UInt32 _size)
	{
		D3D11_MAPPED_SUBRESOURCE data;
		device->GetContext()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		memcpy(data.pData, _data, _size);
		device->GetContext()->Unmap(constantBuffer.Get(), 0);
	}
}