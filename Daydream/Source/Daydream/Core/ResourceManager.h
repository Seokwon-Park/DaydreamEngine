#pragma once

#include "Daydream/Graphics/Managers/MeshManager.h"
#include "Daydream/Graphics/Managers/ModelManager.h"
#include "Daydream/Graphics/Managers/ShaderManager.h"
#include "Daydream/Graphics/Managers/TextureManager.h"
#include "Daydream/Graphics/Managers/RenderPassManager.h"
#include "Daydream/Graphics/Managers/PipelineStateManager.h"

namespace Daydream
{
	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance() {
			static ResourceManager instance;
			return instance;
		}

		static void Init();
	

		inline static void Shutdown()
		{
			GetInstance().meshManager = nullptr;
			GetInstance().textureManager = nullptr;
		}
		
		template <typename T>
		static Shared<T> GetResource(const String& _name) {
			// C++17의 if constexpr을 사용하여 컴파일 타임에 코드를 분기
			if constexpr (std::is_same_v<T, Mesh>) {
				//return GetInstance().meshManager->Get(path);
				return nullptr;
			}
			else if constexpr (std::is_same_v<T, Texture2D>) {
				return GetInstance().textureManager->Get(_name);
			}
			else if constexpr (std::is_same_v<T, Shader>) {
				return GetInstance().shaderManager->Get(_name);
			}
			else if constexpr (std::is_same_v<T, ShaderGroup>) {
				return GetInstance().shaderManager->GetShaderGroup(_name);
			}
			else if constexpr (std::is_same_v<T, RenderPass>) {
				return GetInstance().renderPassManager->Get(_name);
			}
			else if constexpr (std::is_same_v<T, PipelineState>) {
				return GetInstance().pipelineStateManager->Get(_name);
			}
			// 지원하지 않는 타입에 대한 컴파일 타임 에러 처리
			else {
				// static_assert(false, ...)는 컴파일을 무조건 실패시키므로,
				// 항상 false인 의존적인 템플릿 파라미터를 사용해 컴파일을 통과시킨다.
				static_assert(std::is_same_v<T, void>, "Unsupported resource type requested!");
				return nullptr;
			}
		}

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
	private:
		ResourceManager();

		Unique<MeshManager> meshManager;
		Unique<ShaderManager> shaderManager;
		Unique<TextureManager> textureManager;
		Unique<RenderPassManager> renderPassManager;
		Unique<PipelineStateManager> pipelineStateManager;
	};
}