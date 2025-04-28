#include "SteinsPCH.h"
#include "D3D12PipelineState.h"

namespace Steins
{
	D3D12PipelineState::D3D12PipelineState(D3D12GraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
	}

	D3D12PipelineState::~D3D12PipelineState()
	{
	}
}

