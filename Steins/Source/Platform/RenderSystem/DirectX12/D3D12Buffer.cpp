#include "SteinsPCH.h"
#include "D3D12Buffer.h"

namespace Steins
{
	D3D12VertexBuffer::D3D12VertexBuffer(D3D12GraphicsDevice* _device, void* _vertices, UInt32 _size, const BufferLayout& _layout)
	{
		device = _device;
		layout = _layout;
		D3D12_HEAP_PROPERTIES props{};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = _size;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		device->GetDevice()->CreateCommittedResource(&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())
		);

		props.Type = D3D12_HEAP_TYPE_DEFAULT;

		device->GetDevice()->CreateCommittedResource(&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(vertexBuffer.GetAddressOf())
		);

		void* data;
		uploadBuffer.Get()->Map(0, nullptr, &data);
		memcpy(data, _vertices, _size);
		uploadBuffer.Get()->Unmap(0, nullptr);

		device->GetCommandList()->CopyResource(vertexBuffer.Get(), uploadBuffer.Get());

		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = vertexBuffer.Get(); // 전이시킬 리소스
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		device->GetCommandList()->ResourceBarrier(1, &barrier);

		// (4) VB 뷰 생성
		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = (UINT)_size;
		vertexBufferView.StrideInBytes = layout.GetStride();

	}
	void D3D12VertexBuffer::Bind() const
	{
		device->GetCommandList()->IASetVertexBuffers(slot, 1, &vertexBufferView);
	}
	void D3D12VertexBuffer::Unbind() const
	{
	}

	D3D12IndexBuffer::D3D12IndexBuffer(D3D12GraphicsDevice* _device, UInt32* _indices, UInt32 _indexCount)
	{
		indexCount = _indexCount;

		device = _device;
		D3D12_HEAP_PROPERTIES props{};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = sizeof(UInt32) * _indexCount;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		device->GetDevice()->CreateCommittedResource(&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())
		);

		props.Type = D3D12_HEAP_TYPE_DEFAULT;

		device->GetDevice()->CreateCommittedResource(&props,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(indexBuffer.GetAddressOf())
		);

		void* data;
		uploadBuffer.Get()->Map(0, nullptr, &data);
		memcpy(data, _indices, sizeof(UInt32) * _indexCount);
		uploadBuffer.Get()->Unmap(0, nullptr);

		device->GetCommandList()->CopyResource(indexBuffer.Get(), uploadBuffer.Get());

		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = indexBuffer.Get(); // 전이시킬 리소스
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		device->GetCommandList()->ResourceBarrier(1, &barrier);

		// (4) VB 뷰 생성
		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = sizeof(UInt32) * _indexCount;
	}
	void D3D12IndexBuffer::Bind() const
	{
	}
	void D3D12IndexBuffer::Unbind() const
	{
	}
}