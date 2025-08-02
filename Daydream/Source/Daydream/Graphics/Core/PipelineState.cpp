#include "DaydreamPCH.h"
#include "PipelineState.h"

#include "Daydream/Core/Application.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Daydream::PipelineState::PipelineState(const PipelineStateDesc& _desc)
	{
		vertexShader = _desc.vertexShader;
		hullShader = _desc.hullShader;
		domainShader = _desc.domainShader;
		geometryShader = _desc.geometryShader;
		pixelShader = _desc.pixelShader;

		Daydream_CORE_ASSERT(vertexShader != nullptr && pixelShader != nullptr, "VS and PS are necessary!");
		shaders.push_back(vertexShader);
		shaders.push_back(pixelShader);
		if (hullShader != nullptr)shaders.push_back(hullShader);
		if (domainShader != nullptr)shaders.push_back(domainShader);
		if (geometryShader != nullptr)shaders.push_back(geometryShader);

		renderPass = _desc.renderPass;
	}

	Shared<PipelineState> Daydream::PipelineState::Create(const PipelineStateDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreatePipelineState(_desc);
	}
}
