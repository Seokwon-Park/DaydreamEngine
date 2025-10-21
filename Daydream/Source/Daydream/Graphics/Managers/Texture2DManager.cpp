#include "DaydreamPCH.h"
#include "Texture2DManager.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"

#include "Daydream/Core/ResourceManager.h"

namespace Daydream
{
	void Texture2DManager::CreateEssentialTextures()
	{
		TextureDesc desc{};
		desc.bindFlags = RenderBindFlags::ShaderResource;
		desc.width = 1;
		desc.height = 1;
		desc.format = RenderFormat::R8G8B8A8_UNORM;
		UInt32 imageSize = desc.width * desc.height;

		Array<UInt8> pixelData;
		pixelData.resize(imageSize * 4);
		pixelData[3] = 255;

		pixelData[0] = 255;
		pixelData[1] = 255;
		pixelData[2] = 255;
		
		Register("Default", Texture2D::Create(pixelData.data(), desc));
		registry["Default"]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));

		pixelData[0] = 128;
		pixelData[1] = 128;
		pixelData[2] = 255;

		Register("DefaultNormal", Texture2D::Create(pixelData.data(), desc));
		registry["DefaultNormal"]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));

		pixelData[0] = 128;
		pixelData[1] = 128;
		pixelData[2] = 128;
		Register("DefaultRoughness", Texture2D::Create(pixelData.data(), desc));
		registry["DefaultRoughness"]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));

		pixelData[0] = 0;
		pixelData[1] = 0;
		pixelData[2] = 0;
		Register("DefaultMetallic", Texture2D::Create(pixelData.data(), desc));
		registry["DefaultMetallic"]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));

		pixelData[0] = 255;
		pixelData[1] = 255;
		pixelData[2] = 255;
		Register("DefaultAO", Texture2D::Create(pixelData.data(), desc));
		registry["DefaultAO"]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));





	}

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
							pathString.find("_normal.") != std::string::npos ||
							pathString.find("metal") != std::string::npos ||
							pathString.find("rough") != std::string::npos)
						{
							isSRGB = false; // ��ָ��̹Ƿ� ����(Linear)���� ó��
						}

						if (pathString.find("metal") != std::string::npos ||
							pathString.find("rough") != std::string::npos)
						{
							isSRGB = false;
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
						registry[pathString] = Texture2D::Create(data.GetRawDataPtr(), desc);
						registry[pathString]->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
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
