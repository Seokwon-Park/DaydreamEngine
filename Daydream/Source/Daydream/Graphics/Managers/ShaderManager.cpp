#include "DaydreamPCH.h"
#include "ShaderManager.h"

#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Graphics/Resources/ShaderGroup.h"
#include "ShaderGroupManager.h"

namespace Daydream
{
	ShaderManager::ShaderManager()
	{

	}

	void ShaderManager::LoadShadersFromDirectory(Path _directory, bool _isRecursive)
	{
		if (!FileSystem::exists(_directory) || !FileSystem::is_directory(_directory))
		{
			// ��ΰ� ���ų� ���丮�� �ƴϸ� ����
			return;
		}

		const Array<String> supportedExtensions = { ".hlsl" , /* ".frag", ".vert" , ".spv" */};

		// ���丮 ��ȸ
		for (const FileSystem::directory_entry& entry : FileSystem::directory_iterator(_directory))
		{
			if (entry.is_regular_file())
			{
				// �����ϴ� Ȯ�������� Ȯ��
				Path entryPath = entry.path();
				String pathString = entryPath.string();
				String shaderName = entryPath.stem().string();
				String extension = entryPath.extension().string();
				for (const auto& supportedExtension : supportedExtensions) 
				{
					if (extension == supportedExtension) 
					{
						ShaderType shaderType;
						if (pathString.find("VS.") != std::string::npos)
						{
							shaderType = ShaderType::Vertex;
						}
						if (pathString.find("PS.") != std::string::npos)
						{
							shaderType = ShaderType::Pixel;
						}

						if (pathString.find("HS.") != std::string::npos)
						{
							shaderType = ShaderType::Hull;
						}

						if (pathString.find("DS.") != std::string::npos)
						{
							shaderType = ShaderType::Domain;
						}
						if (pathString.find("GS.") != std::string::npos)
						{
							shaderType = ShaderType::Geometry;
						}
						resourceCache[shaderName] = Shader::Create(pathString, shaderType, ShaderLoadMode::File);
						break;
					}
				}
			}
			// ��� �ɼ��� true�̰�, ���� �׸��� ���� ���丮���
			else if (_isRecursive && entry.is_directory())
			{
				// �ڱ� �ڽ��� �ٽ� ȣ���Ͽ� ���� ���� Ž��
				LoadShadersFromDirectory(entry.path(), true);
			}
		}
	}

}
