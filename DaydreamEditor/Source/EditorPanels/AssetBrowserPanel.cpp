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
		//// 예시: 자산 브라우저 패널
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
			// 루트 노드
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
			// 그리드 레이아웃 계산
			float padding = 16.0f;
			float thumbnailSize = 80.0f;
			float cellSize = thumbnailSize + padding;
			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = static_cast<int>(panelWidth / cellSize);
			if (columnCount < 1) columnCount = 1;

			// 테이블 기반 그리드 시작
			if (ImGui::BeginTable("AssetGridTable", columnCount))
			{
				for (const auto& p : FileSystem::directory_iterator(currentPath))
				{
					ImGui::TableNextColumn();

					const Path& path = p.path();
					std::string filenameString = path.filename().string();
					std::string pathString = path.string();

					ImGui::PushID(pathString.c_str()); // 각 위젯에 고유 ID 부여

					// 폴더 또는 파일 아이콘 표시
					if (p.is_directory())
					{
						// 폴더 아이콘 로드 (ResourceManager에 미리 로드해두세요)
						auto texture = ResourceManager::GetResource<Texture2D>("Resource\\DirectoryIcon.png");
						ImGui::ImageButton(pathString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
					}
					else // 파일인 경우
					{
						std::string ext = path.extension().string();
						if (ext == ".png" || ext == ".jpg" || ext == ".hdr") // 이미지 파일
						{
							auto texture = ResourceManager::GetResource<Texture2D>(pathString);
							if (texture != nullptr)
							{
								ImGui::ImageButton(pathString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
							}

							// 드래그 앤 드롭 소스 설정 (ImageButton에만 적용)
							if (ImGui::BeginDragDropSource())
							{
								// 경로 데이터를 페이로드로 설정
								ImGui::SetDragDropPayload("ASSET_TEXTURE", pathString.c_str(), pathString.size() + 1);

								// 드래그 중 미리보기
								ImGui::Text("%s", filenameString.c_str());
								ImGui::Image((ImTextureID)texture->GetImGuiHandle(), ImVec2(50, 50));

								ImGui::EndDragDropSource();
							}
						}
						else // 기타 파일 아이콘
						{
							auto texture = ResourceManager::GetResource<Texture2D>("Resource\\FileIcon.png");
							ImGui::ImageButton(filenameString.c_str(), (ImTextureID)texture->GetImGuiHandle(), { thumbnailSize, thumbnailSize });
						}
					}

					// 더블 클릭 이벤트 처리
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (p.is_directory())
						{
							currentPath /= path.filename(); // 하위 폴더로 이동
						}
					}

					// 파일 이름 표시
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
					RenderFolderTree(entryPath, selectedPath); // 재귀 호출
					ImGui::TreePop();
				}
			}
		}
	}
}

