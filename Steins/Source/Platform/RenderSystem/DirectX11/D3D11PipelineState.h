#pragma once

#include "Steins/Render/PipelineState.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11PipelineState : public PipelineState
	{
	public:
		D3D11PipelineState(GraphicsDevice* _device);
	private:
		D3D11GraphicsDevice* device;
	};
}
