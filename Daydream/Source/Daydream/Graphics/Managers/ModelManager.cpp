#include "DaydreamPCH.h"
#include "ModelManager.h"

#include "Daydream/Graphics/Utility/ModelLoader.h"

namespace Daydream
{
	ModelManager::ModelManager()
	{
	}
	void ModelManager::LoadModelsFromDirectory(Path _directory, bool _isRecursive)
	{
		if (!FileSystem::exists(_directory) || !FileSystem::is_directory(_directory))
		{
			// 경로가 없거나 디렉토리가 아니면 리턴
			return;
		}

		const Array<String> supportedExtensions = { ".gltf", ".fbx" };

		// 디렉토리 순회
		for (FileSystem::directory_entry entry : FileSystem::directory_iterator(_directory))
		{
			if (entry.is_regular_file())
			{
				// 지원하는 확장자인지 확인
				Path entryPath = entry.path();
				String pathString = entryPath.make_preferred().string();
				String extension = entry.path().extension().string();
				for (const auto& supportedExtension : supportedExtensions)
				{
					if (extension == supportedExtension)
					{
						Shared<Model> model = Model::Create();
						model->Load(pathString);
						registry[pathString] = model;
						break;
					}
				}
			}
			// 재귀 옵션이 true이고, 현재 항목이 하위 디렉토리라면
			else if (_isRecursive && entry.is_directory())
			{
				// 자기 자신을 다시 호출하여 하위 폴더 탐색
				LoadModelsFromDirectory(entry.path(), true);
			}
		}
	}
}
