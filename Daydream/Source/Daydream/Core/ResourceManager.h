#pragma once

#include "Daydream/Graphics/Managers/MeshManager.h"
#include "Daydream/Graphics/Managers/ModelManager.h"
#include "Daydream/Graphics/Managers/ShaderManager.h"
#include "Daydream/Graphics/Managers/ShaderGroupManager.h"
#include "Daydream/Graphics/Managers/Texture2DManager.h"
#include "Daydream/Graphics/Managers/SamplerManager.h"
#include "Daydream/Graphics/Managers/RenderPassManager.h"
#include "Daydream/Graphics/Managers/PipelineStateManager.h"

namespace Daydream
{
	#define DECLARE_RESOURCE_GET(ResourceType)	\
	    template<>								\
	    Shared<ResourceType> ResourceManager::GetResource<ResourceType>(const String& _name);
	
	#define DEFINE_RESOURCE_GET(ResourceType, resourceManager)										\
		template<>																					\
		static Shared<ResourceType> GetResource<ResourceType>(const String& _name)	\
		{																							\
			return GetInstance().resourceManager->Get(_name);										\
		}

	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance()
		{
			return *instance;
		}

		static void Init();
		static void Shutdown();
		

		template<typename ResourceType>
		static Shared<ResourceType> GetResource(const String& name);

		DEFINE_RESOURCE_GET(Mesh, meshManager);
		DEFINE_RESOURCE_GET(Shader, shaderManager);
		DEFINE_RESOURCE_GET(ShaderGroup, shaderGroupManager);
		DEFINE_RESOURCE_GET(Sampler, samplerManager);
		DEFINE_RESOURCE_GET(RenderPass, renderPassManager);
		DEFINE_RESOURCE_GET(PipelineState, pipelineStateManager);
		DEFINE_RESOURCE_GET(Texture2D, textureManager);


		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
	private:
		ResourceManager();

		Unique<MeshManager> meshManager;
		Unique<ShaderManager> shaderManager;
		Unique<ShaderGroupManager> shaderGroupManager;
		Unique<Texture2DManager> textureManager;
		Unique<SamplerManager> samplerManager;
		Unique<RenderPassManager> renderPassManager;
		Unique<PipelineStateManager> pipelineStateManager;

		inline static ResourceManager* instance = nullptr;
	};
}