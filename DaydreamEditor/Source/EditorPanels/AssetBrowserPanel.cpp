#include "DaydreamPCH.h"
#include "AssetBrowserPanel.h"

namespace Daydream
{
	static Path rootPath = "Asset";
	AssetBrowserPanel::AssetBrowserPanel()
		:currentPath("Asset")
	{
	}

	AssetBrowserPanel::~AssetBrowserPanel()
	{
	}

	void AssetBrowserPanel::OnImGuiRender()
	{
		//// ����: �ڻ� ������ �г�
		ImGui::Begin("Asset Browser");
		//if (currentPath != rootPath)
		//{
		//	if (ImGui::Button("Back"))
		//	{
		//		currentPath = currentPath.parent_path();
		//	}
		//}

		float treeViewWidth = 200.0f;
		ImGui::BeginChild("FolderTreeView", ImVec2(treeViewWidth, 0));
		{
			// ��Ʈ ���
			if (ImGui::TreeNodeEx(rootPath.string().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth |
				(rootPath == currentPath ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_DrawLinesToNodes))
			{
				if (ImGui::IsItemClicked())
				{
					currentPath = rootPath;
				}
				RenderFolderTree(rootPath, currentPath);
				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("AssetGridView");
		{
			// �׸��� ���̾ƿ� ���
			float padding = 16.0f;
			float thumbnailSize = 80.0f;
			float cellSize = thumbnailSize + padding;
			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = static_cast<int>(panelWidth / cellSize);
			if (columnCount < 1) columnCount = 1;

			// ���̺� ��� �׸��� ����
			if (ImGui::BeginTable("AssetGridTable", columnCount))
			{
				for (const auto& p : FileSystem::directory_iterator(currentPath))
				{
					ImGui::TableNextColumn();

					const Path& path = p.path();
					std::string filenameString = path.filename().string();
					std::string pathString = path.string();

					ImGui::PushID(pathString.c_str()); // �� ������ ���� ID �ο�

					// ���� �Ǵ� ���� ������ ǥ��
					if (p.is_directory())
					{
						// ���� ������ �ε� (ResourceManager�� �̸� �ε��صμ���)
						auto texture = ResourceManager::GetResource<Texture2D>("Resource\\DirectoryIcon.png");
						ImGui::ImageButton(pathString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
					}
					else // ������ ���
					{
						std::string ext = path.extension().string();
						if (ext == ".png" || ext == ".jpg" || ext == ".hdr") // �̹��� ����
						{
							auto texture = ResourceManager::GetResource<Texture2D>(pathString);
							if (texture != nullptr)
							{
								ImGui::ImageButton(pathString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
							}

							// �巡�� �� ��� �ҽ� ���� (ImageButton���� ����)
							if (ImGui::BeginDragDropSource())
							{
								// ��� �����͸� ���̷ε�� ����
								ImGui::SetDragDropPayload("ASSET_TEXTURE", pathString.c_str(), pathString.size() + 1);

								// �巡�� �� �̸�����
								ImGui::Text("%s", filenameString.c_str());
								ImGui::Image((ImTextureID)texture->GetImGuiHandle(), ImVec2(50, 50));

								ImGui::EndDragDropSource();
							}
						}
						else // ��Ÿ ���� ������
						{
							auto texture = ResourceManager::GetResource<Texture2D>("Resource\\FileIcon.png");
							ImGui::ImageButton(filenameString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
						}
					}

					// ���� Ŭ�� �̺�Ʈ ó��
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (p.is_directory())
						{
							currentPath /= path.filename(); // ���� ������ �̵�
						}
					}

					// ���� �̸� ǥ��
					ImGui::TextWrapped("%s", filenameString.c_str());

					ImGui::PopID();
				}
				ImGui::EndTable();
			}
		}
		ImGui::EndChild();
		ImGui::End();

	}

	void AssetBrowserPanel::RenderFolderTree(const Path& path, Path& selectedPath)
	{
		for (const auto& entry : FileSystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				const auto& entryPath = entry.path();
				const std::string filenameString = entryPath.filename().string();

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DrawLinesToNodes;
				if (entryPath == selectedPath) {
					flags |= ImGuiTreeNodeFlags_Selected;
				}

				bool isOpen = ImGui::TreeNodeEx(filenameString.c_str(), flags);

				if (ImGui::IsItemClicked()) {
					selectedPath = entryPath;
				}

				if (isOpen) {
					RenderFolderTree(entryPath, selectedPath); // ��� ȣ��
					ImGui::TreePop();
				}
			}
		}
	}
}

