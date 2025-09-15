#include "PropertyPanel.h"

namespace Daydream
{
	PropertyPanel::PropertyPanel()
	{
	}

	void PropertyPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");
		
		if (selectedEntity)
		{
            char buffer[256];
            strncpy(buffer, selectedEntity->GetName().c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = 0;

            ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;

            if (ImGui::BeginTable("##EntityNameTable", 2, flags)) // 고유 ID, 열 2개, 플래그
            {
                // 열의 속성을 설정합니다. (너비, 크기 조절 등)
                // ImGuiTableColumnFlags_WidthFixed: 초기 너비 고정
                // ImGuiTableColumnFlags_WidthStretch: 창 크기에 따라 너비 조절 (기본값)
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Name"); // 왼쪽 열: 레이블

                ImGui::TableSetColumnIndex(1);
                // [컨트롤러 UI 코드] // 오른쪽 열: 컨트롤러
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2,0 });

                ImGui::SameLine();
                ImGui::InputText("##Name", buffer, selectedEntity->GetName().size());

                ImGui::PopStyleVar();
                ImGui::EndTable();
            }
            ImGui::Separator(); // 이름 입력창 아래에 구분선을 추가
            
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_Framed
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_AllowItemOverlap;

            auto& components = selectedEntity->GetAllComponents();
            for (size_t i = 0; i < components.size(); ++i)
            {
                Component* component = components[i].get();
                const String& name = component->GetName();

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

                bool opened = ImGui::TreeNodeEx((void*)component, flags, "%s", name.c_str());
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("PROPERTY_ITEM", &i, sizeof(size_t));

                    ImGui::Text("%s", name.c_str());

                    ImGui::EndDragDropSource();
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROPERTY_ITEM"))
                    {
                        size_t draggedItemIndex = *(const size_t*)payload->Data;

                        if (draggedItemIndex != i)
                        {
                            components[draggedItemIndex].swap(components[i]);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                if (opened)
                {
                    //draw component Editor
                    DrawComponentProperties(component);
                    ImGui::TreePop();
                }
            }
            ImGui::Button("Add Component");
		}

		ImGui::End();
	}
    void PropertyPanel::DrawComponentProperties(Component* _component)
    {
        const auto& fields = _component->GetFields();

        for (const auto& field : fields)
        {
            switch (field.type)
            {
            case FieldType::Float:
            {
                float* data = static_cast<float*>(field.data);
                ImGui::DragFloat(field.name.c_str(), data, 0.1f);
                break;
            }
            case FieldType::Vector3:
            {
                float* data = static_cast<float*>(field.data);
                ImGui::DragFloat3(field.name.c_str(), data, 0.1f);
                break;
            }
            case FieldType::Transform:
            {
                Transform* data = static_cast<Transform*>(field.data);
                UI::DrawTransformController("Transform", *data);
                break;
            }
            case FieldType::Texture:
            {
                ImGui::Text("%s", field.name.c_str());
                ImGui::SameLine();
                break;
            }
            case FieldType::Light:
            {
                Light* data = static_cast<Light*>(field.data);
                UI::DrawLightController("Light", *data);
                break;
            }
            case FieldType::ModelPtr:
            {
                Model* data = static_cast<Model*>(field.data);
                UI::DrawModelController("Model", *data);
            }
            }
        }
    }
}


