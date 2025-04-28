#include "SteinsPCH.h"
#include "D3D11PipelineState.h"

namespace Steins
{
	D3D11PipelineState::D3D11PipelineState(GraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
	}

	void D3D11PipelineState::Bind() const
	{
	}
}
