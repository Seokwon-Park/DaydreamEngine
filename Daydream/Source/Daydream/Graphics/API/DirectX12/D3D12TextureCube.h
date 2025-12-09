#pragma once

#include "Daydream/Graphics/Resources/TextureCube.h"
#include "D3D12RenderDevice.h"
#include "D3D12Sampler.h"
#include "D3D12ResourceState.h"

namespace Daydream
{
	class D3D12TextureCube : public TextureCube, public D3D12ResourceState, public std::enable_shared_from_this<D3D12TextureCube>
	{
	public:
		// Constrcuter Destructer
		D3D12TextureCube(D3D12RenderDevice* _device, const TextureDesc& _desc);
		virtual ~D3D12TextureCube();

		virtual void SetSampler(Shared<Sampler> _sampler) override;
		virtual bool HasSampler() override { return textureSampler != nullptr; }
		virtual void GenerateMips() override;

		virtual inline void* GetNativeHandle() override { return texture.Get(); }

		UInt32 GetMipLevels() { return mipLevels; }

		ID3D12Resource* GetID3D12Resource() { return texture.Get(); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle() { return srvCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle() { return srvGpuHandle; }
		//inline D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle() { return rtvCpuHandle; }
		//inline const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSVCPUHandle() { return dsvCpuHandle; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle() { return uavCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle() { return uavGpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerHandle() { return textureSampler->GetSamplerHandle(); }

	private:
		D3D12RenderDevice* device;
		D3D12Sampler* textureSampler;

		ComPtr<ID3D12Resource> texture;

		D3D12_CPU_DESCRIPTOR_HANDLE samplerCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE samplerGpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = {};

		Array<D3D12_CPU_DESCRIPTOR_HANDLE> mipSrvCpuHandles = {};
		Array<D3D12_GPU_DESCRIPTOR_HANDLE> mipSrvGpuHandles = {};

		Array<D3D12_CPU_DESCRIPTOR_HANDLE> rtvCpuHandles = {};

		Array<D3D12_CPU_DESCRIPTOR_HANDLE> dsvCpuHandles = {};

		D3D12_CPU_DESCRIPTOR_HANDLE uavCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE uavGpuHandle = {};

		UInt32 mipLevels;
		DXGI_FORMAT format;
		Shared<Material> resizeMaterial;

		Array<Matrix4x4> captureViewProjections;
		Array<Matrix4x4> cubeFaceViewMatrices =
		{
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
			Matrix4x4::CreateLookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f))
		};
		Matrix4x4 cubeFaceProjMatrix = Matrix4x4::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		Array<Shared<ConstantBuffer>> cubeFaceConstantBuffers;
	};
}
