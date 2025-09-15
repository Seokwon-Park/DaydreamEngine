#include "DaydreamPCH.h"
#include "Texture2DManager.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"

#include "Daydream/Core/ResourceManager.h"

namespace Daydream
{
	void Texture2DManager::LoadTexturesFromDirectory(Path _directory, bool _isRecursive)
	{
		if (!FileSystem::exists(_directory) || !FileSystem::is_directory(_directory))
		{
			// ��ΰ� ���ų� ���丮�� �ƴϸ� ����
			return;
		}

		const Array<String> supportedExtensions = { ".png", ".jpg", ".tga", ".hdr", "exr" };

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

						bool isSRGB = true;
						if (pathString.find("_n.") != std::string::npos ||
							pathString.find("_normal.") != std::string::npos)
						{
							isSRGB = false; // ��ָ��̹Ƿ� ����(Linear)���� ó��
						}
						//// �����Ͻ�, ��Ż�� �� �ٸ� ������ �ؽ�ó�� ���� ��Ģ�� �߰�...
						//else if (pathString.find("_r.") != std::string::npos)
						//{
						//	isSRGB = false;
						//}
						ImageLoader::ImageData data = ImageLoader::LoadImageFile(pathString);
						TextureDesc desc{};
						desc.bindFlags = RenderBindFlags::ShaderResource;
						desc.width = data.width;
						desc.height = data.height;
						desc.format = extension == ".hdr" ? RenderFormat::R32G32B32A32_FLOAT : (isSRGB ? RenderFormat::R8G8B8A8_UNORM_SRGB : RenderFormat::R8G8B8A8_UNORM);
						resourceCache[pathString] = Texture2D::Create(data.GetRawDataPtr(), desc);
						resourceCache[pathString]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
						break;
					}
				}
			}
			// ��� �ɼ��� true�̰�, ���� �׸��� ���� ���丮���
			else if (_isRecursive && entry.is_directory())
			{
				// �ڱ� �ڽ��� �ٽ� ȣ���Ͽ� ���� ���� Ž��
				LoadTexturesFromDirectory(entry.path(), true);
			}
		}
	}
}
