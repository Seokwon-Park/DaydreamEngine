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
			// C++17�� if constexpr�� ����Ͽ� ������ Ÿ�ӿ� �ڵ带 �б�
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
			// �������� �ʴ� Ÿ�Կ� ���� ������ Ÿ�� ���� ó��
			else {
				// static_assert(false, ...)�� �������� ������ ���н�Ű�Ƿ�,
				// �׻� false�� �������� ���ø� �Ķ���͸� ����� �������� �����Ų��.
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