#include "DaydreamPCH.h"
#include "ResourceManager.h"


namespace Daydream
{


	ResourceManager::ResourceManager()
	{
		meshManager = MakeUnique<MeshManager>();
		shaderManager = MakeUnique<ShaderManager>();
		shaderGroupManager = MakeUnique<ShaderGroupManager>();
		samplerManager = MakeUnique<SamplerManager>();
		textureManager = MakeUnique<Texture2DManager>();
		renderPassManager = MakeUnique<RenderPassManager>();
		pipelineStateManager = MakeUnique<PipelineStateManager>();
	}

	void ResourceManager::Init()
	{
		GetInstance().textureManager->LoadTexturesFromDirectory("Asset", true);
		GetInstance().shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
		GetInstance().shaderGroupManager->CreateEssentialShaderGroups();
		GetInstance().samplerManager->CreateEssentialSamplers();
		GetInstance().renderPassManager->CreateEssentialRenderPasses();
		GetInstance().pipelineStateManager->CreateEssentialPipelineStates();
	}


}
