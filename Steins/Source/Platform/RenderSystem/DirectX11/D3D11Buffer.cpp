#include "SteinsPCH.h"
#include "D3D11Buffer.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11VertexBuffer::D3D11VertexBuffer(D3D11GraphicsDevice* _device, void* _vertices, UInt32 _size)
	{
		device = _device;

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
		//TODO : VertexArray를 쓰는 방향이 맞나?
		UINT stride = layout.GetStride();
		UINT offset = 0;
		device->GetContext()->IASetVertexBuffers(slot, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
	void D3D11VertexBuffer::Unbind() const
	{
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

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		device->GetDevice()->CreateBuffer(&bufferDesc, &data, indexBuffer.GetAddressOf());
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
}