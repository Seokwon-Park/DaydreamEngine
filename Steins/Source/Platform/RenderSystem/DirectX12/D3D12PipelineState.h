#pragma once

#include "Steins/Render/PipelineState.h"

#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12PipelineState : public PipelineState
	{
	public:
		// Constrcuter Destructer
		D3D12PipelineState(D3D12GraphicsDevice* _device, PipelineStateDesc _desc);
		virtual ~D3D12PipelineState();

		virtual void Bind() const override;
	protected:

	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipeline;


	};
}
