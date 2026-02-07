#include "DaydreamPCH.h"
#include "PipelineStateRegistry.h"

#include "Daydream/Graphics/Resources/PipelineState.h"
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

		//Forward Rendering 
		PipelineStateDesc psoDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Model");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");

		registry["ForwardPSO"] = PipelineState::Create(psoDesc);

		//TODO: Sprite Rendering 
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Sprite");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");

		registry["SpritePSO"] = PipelineState::Create(psoDesc);

		//Resize Texture
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Resize");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["ResizePSO"] = PipelineState::Create(psoDesc);

		//BRDF
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("BRDF");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		registry["BRDFPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Mip"); 
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("MipmapRenderPass");
		registry["GenerateMipsPSO"] = PipelineState::Create(psoDesc);

		// G-Buffer 
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("GBuffer");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("GBufferRenderPass");
		registry["GBufferPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("DeferredLighting");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		registry["DeferredPSO"] = PipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Mask");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("MaskRenderPass");
		registry["MaskPSO"] = PipelineState::Create(psoDesc);

		// Shadow
		RasterizerStateDesc shadowRSDesc = {};
		shadowRSDesc.cullMode = CullMode::Front; 
		shadowRSDesc.fillMode = FillMode::Solid;
		shadowRSDesc.depthBias = 1000;
		shadowRSDesc.slopeScaledDepthBias = 1.0f;
		shadowRSDesc.depthClipEnable = true;

		psoDesc.rasterizerState = shadowRSDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Depth");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("DepthRenderPass");
		registry["DepthPSO"] = PipelineState::Create(psoDesc);


		//Cubemap 렌더링 파이프라인
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Cubemap");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		
		registry["CubemapPSO"] = PipelineState::Create(psoDesc);

		//equirectangular -> Cubemap Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Equirectangular");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["EquirectangularPSO"] = PipelineState::Create(psoDesc);

		// CubemapVS->Irradiance Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Irradiance");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["IrradiancePSO"] = PipelineState::Create(psoDesc);

		// CubemapVS-> Prefilter Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Prefilter");
		psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["PrefilterPSO"] = PipelineState::Create(psoDesc);

	}
}
