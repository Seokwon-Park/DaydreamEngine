#include "PropertyPanel.h"

#include "Daydream/Scene/Components/ComponentRegistry.h"

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

            if (ImGui::BeginTable("##EntityNameTable", 2, flags)) // ���� ID, �� 2��, �÷���
            {
                // ���� �Ӽ��� �����մϴ�. (�ʺ�, ũ�� ���� ��)
                // ImGuiTableColumnFlags_WidthFixed: �ʱ� �ʺ� ����
                // ImGuiTableColumnFlags_WidthStretch: â ũ�⿡ ���� �ʺ� ���� (�⺻��)
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Name"); // ���� ��: ���̺�

                ImGui::TableSetColumnIndex(1);
                // [��Ʈ�ѷ� UI �ڵ�] // ������ ��: ��Ʈ�ѷ�
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2,0 });

                ImGui::SameLine();
                if (ImGui::InputText("##Name", buffer, 256))
                {
                    selectedEntity->SetName(buffer);
                };

                ImGui::PopStyleVar();
                ImGui::EndTable();
            }
            ImGui::Separator(); // �̸� �Է�â �Ʒ��� ���м��� �߰�
            
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
            
            ImGui::Spacing(); // ��ư ���� �ణ�� ���� �߰�
            ImGui::Separator(); // ���м�

            const char* buttonLabel = "Add Component";
            float buttonWidth = ImGui::CalcTextSize(buttonLabel).x + ImGui::GetStyle().FramePadding.x * 2.0f;
            float contentWidth = ImGui::GetContentRegionAvail().x;
            float cursorPosX = (contentWidth - buttonWidth) * 0.5f;
            if (cursorPosX > 0.0f)
            {
                ImGui::SetCursorPosX(cursorPosX);
            }

            if (ImGui::Button(buttonLabel))
            {
                ImGui::OpenPopup("AddComponentPopup");
            }

            if (ImGui::BeginPopup("AddComponentPopup"))
            {
                // ��ϵ� ��� ������Ʈ ����� ������
                const auto& componentFactoryMap = ComponentRegistry::GetComponentMap();

                // �˻� ��� (���� ���������� �ſ� ����)
                static char searchQuery[256] = "";
                ImGui::InputTextWithHint("##Search", "Search Component...", searchQuery, sizeof(searchQuery));
                ImGui::Separator();

                for (const auto& [name, componentFunctions] : componentFactoryMap)
                {
                    String searchQueryLower = searchQuery;
                    std::transform(searchQueryLower.begin(), searchQueryLower.end(), searchQueryLower.begin(), tolower);

                    // 2. �� ����� ������Ʈ �̸�(name)�� �ҹ��ڷ� ��ȯ
                    String nameLower = name;
                    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), tolower);

                    // �˻��� ���͸�
                    if (searchQuery[0] != '\0' && nameLower.find(searchQueryLower) == std::string::npos)
                    {
                        continue; // �˻���� ��ġ���� ������ �ǳʶ�
                    }

                    if (componentFunctions.hasFunc(selectedEntity))
                    {
                        continue;
                    }
                    // ��Ͽ��� ������Ʈ �̸��� Ŭ���ϸ�
                    if (ImGui::Selectable(name.c_str()))
                    {
                        componentFunctions.createFunc(selectedEntity);

                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }
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


