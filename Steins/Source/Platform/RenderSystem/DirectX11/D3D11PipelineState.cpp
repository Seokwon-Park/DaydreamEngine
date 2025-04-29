#include "SteinsPCH.h"
#include "D3D11PipelineState.h"

namespace Steins
{
	D3D11PipelineState::D3D11PipelineState(D3D11GraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		//CW
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = false;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		_device->GetDevice()->CreateRasterizerState(&desc, rasterizer.GetAddressOf());
	}

	void D3D11PipelineState::Bind() const
	{
		for (auto shader : shaders)
		{
			shader->Bind();
		}
		device->GetContext()->RSSetState(rasterizer.Get());

	}
}
