#include "DaydreamPCH.h"
#include "PipelineStateRegistry.h"

#include "Daydream/Graphics/Core/PipelineState.h"
#include "ResourceManager.h"

namespace Daydream
{
	PipelineStateRegistry::PipelineStateRegistry()
	{
	}

	void PipelineStateRegistry::CreateBuiltinResources()
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

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Mip"); // 밉맵 생성용 셰이더 그룹
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("MipmapRenderPass"); // 방금 만든 RenderPass 사용
		registry["GenerateMipsPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("GBuffer"); // 새로 만들 G-Buffer 셰이더
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("GBufferRenderPass"); // 방금 만든 G-Buffer RenderPass
		registry["GBufferPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("DeferredLighting"); 
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass"); 
		registry["DeferredPSO"] = PipelineState::Create(psoDesc);

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
