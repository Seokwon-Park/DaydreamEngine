#pragma once

#include "Daydream/Graphics/Resources/Texture.h"
#include "D3D12RenderDevice.h"
#include "D3D12Sampler.h"
#include "D3D12ResourceState.h"

namespace Daydream
{
	class D3D12Texture2D : public Texture2D, public D3D12ResourceState
	{
	public:
		D3D12Texture2D(D3D12RenderDevice* _device, const TextureDesc& _desc);
		//D3D12Texture2D(D3D12RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc);
		//D3D12Texture2D(D3D12RenderDevice* _device, ComPtr<ID3D12Resource> _texture);
		virtual ~D3D12Texture2D();

		virtual void SetSampler(Shared<Sampler> _sampler) override;

		virtual inline void* GetNativeHandle() override { return texture.Get(); }
		virtual inline void* GetImGuiHandle() { return reinterpret_cast<void*>(GetSRVGPUHandle().ptr); }

		// �� ���� �� ĳ��
		inline ID3D12Resource* GetID3D12Resource() { return texture.Get(); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle() const { return srvCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle() const { return srvGpuHandle; }

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle() const { return rtvCpuHandle; }

		inline const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSVCPUHandle() { return dsvCpuHandle; }

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle() const { return uavCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle() const { return uavGpuHandle; }

		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerHandle() { return textureSampler->GetSamplerHandle(); }
		//const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpuHandle; };
	private:
		D3D12RenderDevice* device;
		D3D12Sampler* textureSampler;

		ComPtr<ID3D12Resource> texture;

		D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE uavCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE uavGpuHandle = {};
	};
}
