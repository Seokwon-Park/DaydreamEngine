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

		for (const auto& info : shaderGroup->GetShaderResourceData())
		{
			switch (info.set)
			{
			case 0:
				globalBindingMap[info.name] = info;
				break;
			case 1:
				materialBindingMap[info.name] = info;
				break;
			case 2:
				entityBindingMap[info.name] = info;
				break;
			}
		}
	}

	Shared<PipelineState> PipelineState::Create(const PipelineStateDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreatePipelineState(_desc);
	}
}
