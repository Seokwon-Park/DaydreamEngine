#include "DaydreamPCH.h"
#include "PipelineState.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	PipelineState::PipelineState(const PipelineStateDesc& _desc)
	{
		desc = _desc;
		shaderGroup = _desc.shaderGroup;
		renderPass = _desc.renderPass;
	}

	Shared<PipelineState> PipelineState::Create(const PipelineStateDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreatePipelineState(_desc);
	}
}
