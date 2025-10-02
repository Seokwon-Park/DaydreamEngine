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
		DAYDREAM_CORE_ASSERT(!instance, "ResourceManager must be initialized only 1 time");
		instance = new ResourceManager();

		instance->samplerManager->CreateEssentialSamplers();
		instance->textureManager->LoadTexturesFromDirectory("Asset", true);
		instance->textureManager->LoadTexturesFromDirectory("Resource", true);
		instance->shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
		instance->shaderGroupManager->CreateEssentialShaderGroups();
		instance->renderPassManager->CreateEssentialRenderPasses();
		instance->pipelineStateManager->CreateEssentialPipelineStates();
	}

	void ResourceManager::Shutdown()
	{
		instance->pipelineStateManager = nullptr;
		instance->renderPassManager = nullptr;
		instance->textureManager = nullptr;
		instance->samplerManager = nullptr;
		instance->shaderGroupManager = nullptr;
		instance->shaderManager = nullptr;
		instance->meshManager = nullptr;

		delete instance;
	}
}
