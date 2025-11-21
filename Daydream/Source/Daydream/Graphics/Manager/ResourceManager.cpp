#include "DaydreamPCH.h"
#include "ResourceManager.h"

#include "Daydream/Graphics/Resources/ShaderGroup.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Resources/RenderPass.h"
#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Graphics/Resources/Mesh.h"
#include "Daydream/Graphics/Resources/Sampler.h"

#include "ShaderRegistry.h"
#include "ShaderGroupRegistry.h"
#include "RenderPassRegistry.h"
#include "MeshRegistry.h"
#include "PipelineStateRegistry.h"
#include "SamplerRegistry.h"



namespace Daydream
{
	ResourceManager::ResourceManager()
	{

	}

	void ResourceManager::Init()
	{
		DAYDREAM_CORE_ASSERT(!instance, "ResourceManager must be initialized only 1 time");
		instance = new ResourceManager();

		instance->registryList[typeid(ShaderGroup)] = MakeUnique<ShaderGroupRegistry>();
		instance->registryList[typeid(RenderPass)] = MakeUnique<RenderPassRegistry>();
		instance->registryList[typeid(Sampler)] = MakeUnique<SamplerRegistry>();
		instance->registryList[typeid(Mesh)] = MakeUnique<MeshRegistry>();
		instance->registryList[typeid(PipelineState)] = MakeUnique<PipelineStateRegistry>();

		for (auto& [id, registry] : instance->registryList)
		{
			registry->CreateBuiltinResources();
		}

		////instance->meshManager->CreateEssentialMeshes();
		//instance->samplerManager->CreateEssentialSamplers();
		//instance->textureManager->CreateEssentialTextures();
		//instance->textureManager->LoadTexturesFromDirectory("Asset", true);
		//instance->textureManager->LoadTexturesFromDirectory("Resource", true);
		//instance->shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
		////instance->shaderGroupManager->Init();
		//instance->renderPassManager->CreateEssentialRenderPasses();
		//instance->pipelineStateManager->CreateEssentialPipelineStates();
		//instance->modelManager->LoadModelsFromDirectory("Asset", true);
	}

	void ResourceManager::Shutdown()
	{
		instance->registryList.clear();

		delete instance;
	}

	//ResourceManager::ResourceManager()
	//{
	//	//meshManager = MakeUnique<MeshManager>();
	//	//modelManager = MakeUnique<ModelManager>();
	//	//shaderManager = MakeUnique<ShaderManager>();
	//	//shaderGroupManager = MakeUnique<ShaderGroupRegistry>();
	//	//samplerManager = MakeUnique<SamplerRegistry>();
	//	//textureManager = MakeUnique<Texture2DManager>();
	//	//renderPassManager = MakeUnique<RenderPassManager>();
	//	//pipelineStateManager = MakeUnique<PipelineStateManager>();
	//}

	//void ResourceManager::Init()
	//{
	//	DAYDREAM_CORE_ASSERT(!instance, "ResourceManager must be initialized only 1 time");
	//	instance = new ResourceManager();

	//	//	//instance->meshManager->CreateEssentialMeshes();
	//	//	instance->samplerManager->CreateEssentialSamplers();
	//	//	instance->textureManager->CreateEssentialTextures();
	//	//	instance->textureManager->LoadTexturesFromDirectory("Asset", true);
	//	//	instance->textureManager->LoadTexturesFromDirectory("Resource", true);
	//	//	instance->shaderManager->LoadShadersFromDirectory("Asset/Shader", true);
	//	//	//instance->shaderGroupManager->Init();
	//	//	instance->renderPassManager->CreateEssentialRenderPasses();
	//	//	instance->pipelineStateManager->CreateEssentialPipelineStates();
	//	//	instance->modelManager->LoadModelsFromDirectory("Asset", true);
	//	//}

	//	//void ResourceManager::Shutdown()
	//	//{
	//	//	instance->pipelineStateManager = nullptr;
	//	//	instance->renderPassManager = nullptr;
	//	//	instance->textureManager = nullptr;
	//	//	instance->samplerManager = nullptr;
	//	//	instance->shaderGroupManager = nullptr;
	//	//	instance->shaderManager = nullptr;
	//	//	instance->meshManager = nullptr;

	//	delete instance;
	//}
}

