#pragma once

#include "Daydream/Graphics/Resources/Sampler.h"

#include "D3D11RenderDevice.h"

namespace Daydream
{
	class D3D11Sampler : public Sampler
	{
	public:
		D3D11Sampler(D3D11RenderDevice* _device, const SamplerDesc& _desc);
		virtual ~D3D11Sampler() override {};

		ComPtr<ID3D11SamplerState> GetSampler() { return sampler; }
	private:
		D3D11RenderDevice* device;

		ComPtr<ID3D11SamplerState> sampler;
		
	};
}