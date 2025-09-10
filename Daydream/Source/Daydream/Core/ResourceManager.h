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
		static ResourceManager& GetInstance() {
			static ResourceManager instance;
			return instance;
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

		//template <typename T>
		//static Shared<T> GetResource(const String& _name) {
		//	// C++17의 if constexpr을 사용하여 컴파일 타임에 코드를 분기
		//	if constexpr (std::is_same_v<T, Mesh>) {
		//		//return GetInstance().meshManager->Get(path);
		//		return nullptr;
		//	}
		//	else if constexpr (std::is_same_v<T, Texture2D>) {
		//		return GetInstance().textureManager->Get(_name);
		//	}
		//	else if constexpr (std::is_same_v<T, Shader>) {
		//		return GetInstance().shaderManager->Get(_name);
		//	}
		//	else if constexpr (std::is_same_v<T, ShaderGroup>) {
		//		return GetInstance().shaderGroupManager->Get(_name);
		//	}
		//	else if constexpr (std::is_same_v<T, ShaderGroup>) {
		//		return GetInstance().shaderGroupManager->Get(_name);
		//	}
		//	else if constexpr (std::is_same_v<T, RenderPass>) {
		//		return GetInstance().renderPassManager->Get(_name);
		//	}
		//	else if constexpr (std::is_same_v<T, PipelineState>) {
		//		return GetInstance().pipelineStateManager->Get(_name);
		//	}
		//	// 지원하지 않는 타입에 대한 컴파일 타임 에러 처리
		//	else {
		//		// static_assert(false, ...)는 컴파일을 무조건 실패시키므로,
		//		// 항상 false인 의존적인 템플릿 파라미터를 사용해 컴파일을 통과시킨다.
		//		static_assert(std::is_same_v<T, void>, "Unsupported resource type requested!");
		//		return nullptr;
		//	}
		//}

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
	};
}