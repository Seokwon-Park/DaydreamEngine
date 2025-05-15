#pragma once

#include "Steins/Render/Texture.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12Texture2D : public Texture2D
	{
	public:
		D3D12Texture2D(D3D12GraphicsDevice* _device, const FilePath& _path);

		virtual void Bind(UInt32 _slot) const override;

		virtual void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<size_t>(gpuHandle.ptr)); };
	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12Resource> uploadBuffer;
		ComPtr<ID3D12Resource> texture;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};
}
