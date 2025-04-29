#pragma once

#include "Steins/Render/PipelineState.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11PipelineState : public PipelineState
	{
	public:
		D3D11PipelineState(D3D11GraphicsDevice* _device, PipelineStateDesc _desc);

		virtual void Bind() const override;
	private:
		D3D11GraphicsDevice* device;
		ComPtr<ID3D11RasterizerState> rasterizer;

	};
}
