#include "SteinsPCH.h"
#include "D3D11PipelineState.h"

namespace Steins
{
	D3D11PipelineState::D3D11PipelineState(D3D11RenderDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		//CW
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = false;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		_device->GetDevice()->CreateRasterizerState(&desc, rasterizer.GetAddressOf());

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->GetDevice()->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());
	}

	void D3D11PipelineState::Bind() const
	{
		for (auto shader : shaders)
		{
			shader->Bind();
		}
		device->GetContext()->RSSetState(rasterizer.Get());
		device->GetContext()->PSSetSamplers(0, 1, sampler.GetAddressOf());
	}
}
