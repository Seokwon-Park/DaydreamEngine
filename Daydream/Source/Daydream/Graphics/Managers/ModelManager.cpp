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
			// ��ΰ� ���ų� ���丮�� �ƴϸ� ����
			return;
		}

		const Array<String> supportedExtensions = { ".gltf", ".fbx" };

		// ���丮 ��ȸ
		for (FileSystem::directory_entry entry : FileSystem::directory_iterator(_directory))
		{
			if (entry.is_regular_file())
			{
				// �����ϴ� Ȯ�������� Ȯ��
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
			// ��� �ɼ��� true�̰�, ���� �׸��� ���� ���丮���
			else if (_isRecursive && entry.is_directory())
			{
				// �ڱ� �ڽ��� �ٽ� ȣ���Ͽ� ���� ���� Ž��
				LoadModelsFromDirectory(entry.path(), true);
			}
		}
	}
}
