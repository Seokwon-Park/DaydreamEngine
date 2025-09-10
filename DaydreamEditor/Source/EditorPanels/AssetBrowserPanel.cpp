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
		// ����: �ڻ� ������ �г�
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
		// ���߿� ���� �ý��� Ž�� �� �ڻ� �̸�����
		ImGui::End();

	}
}

