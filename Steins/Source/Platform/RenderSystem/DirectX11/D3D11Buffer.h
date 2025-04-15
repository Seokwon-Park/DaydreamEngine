#pragma once

#include "Steins/Render/Buffer.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(GraphicsDevice* _device, Float32* _vertices, UInt32 _size);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		void SetSlotNum(UInt32 _slotNum) { slotNum = _slotNum; }
	private:
		D3D11GraphicsDevice* device;
		UInt32 slotNum = 0;
		ComPtr<ID3D11Buffer> vertexBuffer;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual UInt32 GetCount() const { return indexCount; }
	private:
		D3D11GraphicsDevice* device;
		ComPtr<ID3D11Buffer> indexBuffer;
		UInt32 indexCount;
	};
}

