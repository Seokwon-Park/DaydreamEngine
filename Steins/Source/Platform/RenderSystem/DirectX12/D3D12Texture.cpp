#include "SteinsPCH.h"
#include "D3D12Texture.h"

#include "stb_image.h"

namespace Steins
{
	D3D12Texture2D::D3D12Texture2D(D3D12GraphicsDevice* _device, const FilePath& _path)
		:Texture2D(_path)
	{
		device = _device;

		data = stbi_load(_path.ToString().c_str(), &width, &height, &channels, 0);
		STEINS_CORE_ASSERT(data, "Failed to load image!");

		UInt8* newPixels = new UInt8[width * height * 4];
		if (channels == 3)
		{
			for (int i = 0; i < width * height; i++)
			{
				newPixels[i * 4] = data[i * 3];
				newPixels[i * 4 + 1] = data[i * 3 + 1];
				newPixels[i * 4 + 2] = data[i * 3 + 2];
				newPixels[i * 4 + 3] = 255;
			}
		}
		else if (channels == 4)
		{
			memcpy(newPixels, data, width * height * 4 * sizeof(UInt8));
		}

		D3D12_HEAP_PROPERTIES props{};
		props.Type = D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = width;
		desc.Height = height;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		device->GetDevice()->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(texture.GetAddressOf()));

		UInt32 uploadPitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
		UInt32 uploadSize = height * uploadPitch;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = uploadSize;
		desc.Height = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		props.Type = D3D12_HEAP_TYPE_UPLOAD;

		device->GetDevice()->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf()));

		void* pixelData;
		D3D12_RANGE range = { 0, uploadSize };
		uploadBuffer->Map(0, &range, &pixelData);
		for (int y = 0; y < height; y++)
		{
			memcpy(reinterpret_cast<UInt8*>(pixelData) + y * uploadPitch, data + y * width * 4, width * 4);
		}

		// Copy the upload resource content into the real resource
		D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
		srcLocation.pResource = uploadBuffer.Get();
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srcLocation.PlacedFootprint.Footprint.Width = width;
		srcLocation.PlacedFootprint.Footprint.Height = height;
		srcLocation.PlacedFootprint.Footprint.Depth = 1;
		srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

		D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
		dstLocation.pResource = texture.Get();
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = 0;

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = texture.Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		device->GetCommandList()->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
		device->GetCommandList()->ResourceBarrier(1, &barrier);

		 

	}
}
