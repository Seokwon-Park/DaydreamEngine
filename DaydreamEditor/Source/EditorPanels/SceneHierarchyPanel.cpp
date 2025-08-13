#include "SceneHierarchyPanel.h"

namespace Daydream
{
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
	}
    void SceneHierarchyPanel::SetCurrentScene(Scene* _scene)
    {
        scene = _scene;
    }

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

        ImGui::InputText("##Search", searchQuery, sizeof(searchQuery), ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Search for entities by name.");
        ImGui::Separator();

        if (scene) 
        {
            auto& entities = scene->GetAllEntities();

            for (size_t i = 0; i < entities.size(); ++i)
            {
                GameEntity* gameEntity = entities[i].get();
                const String& name = gameEntity->GetName();

                ImGuiTreeNodeFlags flags = ((selectedEntity == gameEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                
                bool opened = ImGui::TreeNodeEx((void*)gameEntity, flags, "%s", name.c_str());

                if (ImGui::IsItemClicked())
                {
                    selectedEntity = gameEntity;
                }

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", &i, sizeof(size_t));

                    ImGui::Text("%s", name.c_str());

                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
                    {
                        size_t draggedItemIndex = *(const size_t*)payload->Data;

                        if (draggedItemIndex != i)
                        {
                            entities[draggedItemIndex].swap(entities[i]);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                if (opened)
                {
                    ImGui::TreePop();
                }
            }
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered()) {
                selectedEntity = nullptr; // 선택 해제
            }
        }

        if (ImGui::BeginPopupContextWindow("SceneHierarchyPopup"))
        {
            if (ImGui::MenuItem("Create Empty GameEntity"))
            {
                scene->CreateGameEntity();
            }
            if (ImGui::MenuItem("Create Camera"))
            {
            }

            ImGui::Separator(); 

            if (ImGui::MenuItem("Close"))
            {
            }

            ImGui::EndPopup();
        }

		ImGui::End();
	}
}
