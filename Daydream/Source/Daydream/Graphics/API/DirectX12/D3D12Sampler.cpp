#include "DaydreamPCH.h"
#include "D3D12Sampler.h"

#include "D3D12Utility.h"

namespace Daydream
{
	D3D12Sampler::D3D12Sampler(D3D12RenderDevice* _device, const SamplerDesc& _desc)
	{
		device = _device;

		D3D12_SAMPLER_DESC samplerDesc = GraphicsUtility::DirectX12::TranslateToD3D12SamplerDesc(_desc);

		device->GetSamplerHeapAlloc().Alloc(&samplerCpuHandle, &samplerGpuHandle);

		device->GetDevice()->CreateSampler(&samplerDesc, samplerCpuHandle);
	}

	D3D12Sampler::~D3D12Sampler()
	{
	}
}

