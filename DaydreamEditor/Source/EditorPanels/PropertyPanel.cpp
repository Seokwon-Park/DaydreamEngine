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
            }
        }
    }
}


