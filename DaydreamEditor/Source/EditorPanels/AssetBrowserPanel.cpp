#include "DaydreamPCH.h"
#include "AssetBrowserPanel.h"

namespace Daydream
{
	static Path assetDirectory = "Asset";
	AssetBrowserPanel::AssetBrowserPanel()
		:currentPath("Asset")
	{
	}

	AssetBrowserPanel::~AssetBrowserPanel()
	{
	}
	void AssetBrowserPanel::OnImGuiRender()
	{
		// 예시: 자산 브라우저 패널
		ImGui::Begin("Asset Browser");
		if (currentPath != assetDirectory)
		{
			if (ImGui::Button("Back"))
			{
				currentPath = currentPath.parent_path();
			}
		}

		for (auto& p : FileSystem::directory_iterator(currentPath))
		{
			Path path = p.path();
			String filenameString = path.filename().string();
			String ext = path.extension().string();
			if (path.has_extension() && (ext == ".png" || ext == ".jpg"))
			{
				auto texture = ResourceManager::GetResource<Texture2D>(path.string());
				ImGui::Image(texture->GetImGuiHandle(), { 50,50 });
			}
			ImGui::Button(filenameString.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (p.is_directory())
				{
					currentPath /= path.filename();
				}
			}
		
		}
		// 나중에 파일 시스템 탐색 및 자산 미리보기
		ImGui::End();

	}
}

