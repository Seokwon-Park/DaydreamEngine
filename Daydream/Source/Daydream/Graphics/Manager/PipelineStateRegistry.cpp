#include "DaydreamPCH.h"
#include "PipelineStateRegistry.h"

#include "Daydream/Graphics/Resources/PipelineState/GraphicsPipelineState.h"
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
		GraphicsPipelineStateDesc psoDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Model");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R8G8B8A8_UNORM
		};
		psoDesc.depthStencilFormat =
		{
			RenderFormat::R24G8_TYPELESS
		};
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		registry["ForwardPSO"] = GraphicsPipelineState::Create(psoDesc);

		//TODO: Sprite Rendering 
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Sprite");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R8G8B8A8_UNORM
		};
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		registry["SpritePSO"] = GraphicsPipelineState::Create(psoDesc);

		//Resize Texture
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Resize");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R16G16B16A16_FLOAT
		};
		registry["ResizePSO"] = GraphicsPipelineState::Create(psoDesc);

		//BRDF
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("BRDF");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		registry["BRDFPSO"] = GraphicsPipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Mip"); 
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("MipmapRenderPass");
		registry["GenerateMipsPSO"] = GraphicsPipelineState::Create(psoDesc);

		// G-Buffer 
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("GBuffer");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R16G16B16A16_FLOAT,
			RenderFormat::R16G16B16A16_FLOAT,
			RenderFormat::R8G8B8A8_UNORM,
			RenderFormat::R8G8B8A8_UNORM,
			RenderFormat::R32_UINT,
		};
		psoDesc.depthStencilFormat =
		{
			RenderFormat::R24G8_TYPELESS
		};
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("GBufferRenderPass");
		registry["GBufferPSO"] = GraphicsPipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("DeferredLighting");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		registry["DeferredPSO"] = GraphicsPipelineState::Create(psoDesc);

		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Mask");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("MaskRenderPass");
		registry["MaskPSO"] = GraphicsPipelineState::Create(psoDesc);

		// Shadow
		RasterizerStateDesc shadowRSDesc = {};
		shadowRSDesc.cullMode = CullMode::Front; 
		shadowRSDesc.fillMode = FillMode::Solid;
		shadowRSDesc.depthBias = 1000;
		shadowRSDesc.slopeScaledDepthBias = 1.0f;
		shadowRSDesc.depthClipEnable = true;

		psoDesc.rasterizerState = shadowRSDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Depth");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("DepthRenderPass");


		registry["DepthPSO"] = GraphicsPipelineState::Create(psoDesc);


		//Cubemap 렌더링 파이프라인
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Cubemap");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("StandardRenderPass");
		
		registry["CubemapPSO"] = GraphicsPipelineState::Create(psoDesc);

		//equirectangular -> Cubemap Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Equirectangular");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R16G16B16A16_FLOAT
		};
		psoDesc.depthStencilFormat =
		{
			RenderFormat::R24G8_TYPELESS
		};
		registry["EquirectangularPSO"] = GraphicsPipelineState::Create(psoDesc);

		// CubemapVS->Irradiance Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Irradiance");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
		psoDesc.renderTargetFormats =
		{
			RenderFormat::R16G16B16A16_FLOAT
		};
		psoDesc.depthStencilFormat =
		{
			RenderFormat::R24G8_TYPELESS
		};

		registry["IrradiancePSO"] = GraphicsPipelineState::Create(psoDesc);

		// CubemapVS-> Prefilter Faces
		psoDesc.rasterizerState = rastDesc;
		psoDesc.shaderGroup = ResourceManager::GetResource<ShaderGroup>("Prefilter");
		//psoDesc.renderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");

		registry["PrefilterPSO"] = GraphicsPipelineState::Create(psoDesc);

	}
}
