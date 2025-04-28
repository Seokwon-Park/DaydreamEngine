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

	protected:

	private:

	};
}
