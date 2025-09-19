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

		GetInstance().samplerManager->CreateEssentialSamplers();
		GetInstance().textureManager->LoadTexturesFromDirectory("Asset", true);
		GetInstance().textureManager->LoadTexturesFromDirectory("Resource", true);
		GetInstance().shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
		GetInstance().shaderGroupManager->CreateEssentialShaderGroups();
		GetInstance().renderPassManager->CreateEssentialRenderPasses();
		GetInstance().pipelineStateManager->CreateEssentialPipelineStates();
	}

	void ResourceManager::Shutdown()
	{
		GetInstance().pipelineStateManager = nullptr;
		GetInstance().renderPassManager = nullptr;
		GetInstance().textureManager = nullptr;
		GetInstance().samplerManager = nullptr;
		GetInstance().shaderGroupManager = nullptr;
		GetInstance().shaderManager = nullptr;
		GetInstance().meshManager = nullptr;

		delete instance;
	}
}
