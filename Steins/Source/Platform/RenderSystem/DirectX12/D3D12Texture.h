#pragma once

#include "Steins/Render/Texture.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12Texture2D : public Texture2D
	{
	public:
		D3D12Texture2D(D3D12GraphicsDevice* _device, const FilePath& _path);
	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12Resource> uploadBuffer;
		ComPtr<ID3D12Resource> texture;
	};
}
