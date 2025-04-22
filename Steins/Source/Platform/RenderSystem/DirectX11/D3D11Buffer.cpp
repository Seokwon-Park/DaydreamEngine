#include "SteinsPCH.h"
#include "D3D11Buffer.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11VertexBuffer::D3D11VertexBuffer(D3D11GraphicsDevice* _device, Float32* _vertices, UInt32 _size)
	{
		device = _device;
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = _size;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		device->GetDevice()->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
	}
	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
	}
	void D3D11VertexBuffer::Bind() const
	{
		//TODO : VertexArray를 쓰는 방향이 맞나?
		UINT stride = layout.GetStride();
		UINT offset = 0;
		device->GetContext()->IASetVertexBuffers(slotNum, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	}
	void D3D11VertexBuffer::Unbind() const
	{
	}
	D3D11IndexBuffer::D3D11IndexBuffer(D3D11GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount)
	{
		device = _device;
		indexCount = _indexCount;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(UInt32) * indexCount;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		device->GetDevice()->CreateBuffer(&desc, &data, indexBuffer.GetAddressOf());
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