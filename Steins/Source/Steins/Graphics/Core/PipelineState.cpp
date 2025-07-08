#include "SteinsPCH.h"
#include "PipelineState.h"

#include "Steins/Core/Application.h"

#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
{
	PipelineState::PipelineState(PipelineStateDesc _desc)
	{
		vertexShader = _desc.vertexShader;
		hullShader = _desc.hullShader;
		domainShader = _desc.domainShader;
		geometryShader = _desc.geometryShader;
		pixelShader = _desc.pixelShader;

		STEINS_CORE_ASSERT(vertexShader != nullptr && pixelShader != nullptr, "VS and PS are necessary!");
		shaders.push_back(vertexShader);
		shaders.push_back(pixelShader);
		if (hullShader != nullptr)shaders.push_back(hullShader);
		if (domainShader != nullptr)shaders.push_back(domainShader);
		if (geometryShader != nullptr)shaders.push_back(geometryShader);
	}

	Shared<PipelineState> PipelineState::Create(PipelineStateDesc _desc)
	{
		return Renderer::GetRenderDevice()->CreatePipelineState(_desc);
	}
}
