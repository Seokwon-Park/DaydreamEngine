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
			// 경로가 없거나 디렉토리가 아니면 리턴
			return;
		}

		const Array<String> supportedExtensions = { ".png", ".jpg", ".tga", ".hdr", "exr" };

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

						bool isSRGB = true;
						if (pathString.find("_n.") != std::string::npos ||
							pathString.find("_normal.") != std::string::npos ||
							pathString.find("metal") != std::string::npos ||
							pathString.find("rough") != std::string::npos)
						{
							isSRGB = false; // 노멀맵이므로 선형(Linear)으로 처리
						}

						if (pathString.find("metal") != std::string::npos ||
							pathString.find("rough") != std::string::npos)
						{
							isSRGB = false;
						}
						//// 러프니스, 메탈릭 등 다른 데이터 텍스처에 대한 규칙도 추가...
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
			// 재귀 옵션이 true이고, 현재 항목이 하위 디렉토리라면
			else if (_isRecursive && entry.is_directory())
			{
				// 자기 자신을 다시 호출하여 하위 폴더 탐색
				LoadTexturesFromDirectory(entry.path(), true);
			}
		}
	}
}
