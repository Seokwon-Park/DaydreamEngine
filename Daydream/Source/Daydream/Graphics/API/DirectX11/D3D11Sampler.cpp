#include "DaydreamPCH.h"
#include "D3D11Sampler.h"

#include "D3D11Utility.h"

namespace Daydream
{
	D3D11Sampler::D3D11Sampler(D3D11RenderDevice* _device, const SamplerDesc& _desc)
	{
		device = _device;

		D3D11_SAMPLER_DESC desc = GraphicsUtility::DirectX11::TranslateToD3D11SamplerDesc(_desc);
		device->GetDevice()->CreateSamplerState(&desc, sampler.GetAddressOf());
	}
}
