#include "DaydreamPCH.h"
#include "Texture2DManager.h"

namespace Daydream
{
	void Texture2DManager::LoadTexturesFromDirectory(Path _directory, bool _isRecursive)
	{
		if (!FileSystem::exists(_directory) || !FileSystem::is_directory(_directory))
		{
			// 경로가 없거나 디렉토리가 아니면 리턴
			return;
		}

		const Array<String> supportedExtensions = { ".png", ".jpg", ".tga" };

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
						TextureDesc textureDesc{};
						textureDesc.bindFlags = RenderBindFlags::ShaderResource;
						bool isSRGB = true;
						if (pathString.find("_n.") != std::string::npos ||
							pathString.find("_normal.") != std::string::npos)
						{
							isSRGB = false; // 노멀맵이므로 선형(Linear)으로 처리
						}
						//// 러프니스, 메탈릭 등 다른 데이터 텍스처에 대한 규칙도 추가...
						//else if (pathString.find("_r.") != std::string::npos)
						//{
						//	isSRGB = false;
						//}
						textureDesc.format = isSRGB ? RenderFormat::R8G8B8A8_UNORM_SRGB : RenderFormat::R8G8B8A8_UNORM;
						resourceCache[pathString] = Texture2D::Create(pathString, textureDesc);
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
