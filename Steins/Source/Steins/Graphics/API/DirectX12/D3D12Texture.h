#pragma once

#include "Steins/Graphics/Resources/Texture.h"
#include "D3D12RenderDevice.h"

namespace Steins
{
	class D3D12Texture2D : public Texture2D
	{
	public:
		D3D12Texture2D(D3D12RenderDevice* _device, const TextureDesc& _desc);
		D3D12Texture2D(D3D12RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc);
		D3D12Texture2D(D3D12RenderDevice* _device, ComPtr<ID3D12Resource> _texture);

		virtual inline void* GetNativeHandle() override { return texture.Get(); }

		// ºä »ý¼º ¹× Ä³½Ì
		ComPtr<ID3D12Resource> GetTexture() { return texture; }
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle();
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle();
		D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle();
		virtual inline void* GetImGuiHandle() { return reinterpret_cast<void*>(srvGpuHandle.ptr); }
		//const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpuHandle; };


	private:
		D3D12RenderDevice* device;
		ComPtr<ID3D12Resource> uploadBuffer;
		ComPtr<ID3D12Resource> texture;
		D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = {};
		
		D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE uavCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE uavGpuHandle = {};
	};
}
