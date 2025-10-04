#include "DaydreamPCH.h"
#include "PipelineStateManager.h"

#include "Daydream/Graphics/Core/PipelineState.h"
#include "Daydream/Core/ResourceManager.h"

namespace Daydream
{
	PipelineStateManager::PipelineStateManager()
	{
	}

	void PipelineStateManager::CreateEssentialPipelineStates()
	{
		RasterizerStateDesc rastDesc;
		rastDesc.frontCounterClockwise = false;
		rastDesc.cullMode = CullMode::Front;
		rastDesc.fillMode = FillMode::Solid;

		PipelineStateDesc psoDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Model");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");

		registry["ForwardPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Sprite");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");

		registry["SpritePSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Resize");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["ResizePSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("BRDF");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["BRDFPSO"] = PipelineState::Create(psoDesc);

		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Cubemap");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		
		registry["CubemapPSO"] = PipelineState::Create(psoDesc);

		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Equirectangular");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["EquirectangularPSO"] = PipelineState::Create(psoDesc);

		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Irradiance");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["IrradiancePSO"] = PipelineState::Create(psoDesc);

		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Prefilter");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["PrefilterPSO"] = PipelineState::Create(psoDesc);

	}
}
