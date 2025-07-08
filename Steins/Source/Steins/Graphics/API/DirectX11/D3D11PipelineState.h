#pragma once

#include "Steins/Graphics/Core/PipelineState.h"
#include "D3D11RenderDevice.h"

namespace Steins
{
	class D3D11PipelineState : public PipelineState
	{
	public:
		D3D11PipelineState(D3D11RenderDevice* _device, PipelineStateDesc _desc);

		virtual void Bind() const override;
		virtual Shared<Material> CreateMaterial() override { return nullptr; };
	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11RasterizerState> rasterizer;
		ComPtr<ID3D11SamplerState> sampler;

	};
}
