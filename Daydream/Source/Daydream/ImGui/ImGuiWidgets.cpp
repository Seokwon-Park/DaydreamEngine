#include "DaydreamPCH.h"
#include "ImGuiWidgets.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Core/ResourceManager.h"

namespace Daydream::UI
{
	void DrawDragFloat(const String& _label, Float32& _value, Float32 _speed, Float32 _minValue, Float32 _maxValue, Float32 _resetValue, Float32 _columnWidth)
	{
		ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;

		if (ImGui::BeginTable(_label.c_str(), 2, flags)) // ���� ID, �� 2��, �÷���
		{
			// ���� �Ӽ��� �����մϴ�. (�ʺ�, ũ�� ���� ��)
			// ImGuiTableColumnFlags_WidthFixed: �ʱ� �ʺ� ����
			// ImGuiTableColumnFlags_WidthStretch: â ũ�⿡ ���� �ʺ� ���� (�⺻��)
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, _columnWidth);
			ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(_label.c_str()); // ���� ��: ���̺�

			ImGui::TableSetColumnIndex(1);
			// [��Ʈ�ѷ� UI �ڵ�] // ������ ��: ��Ʈ�ѷ�
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2,0 });
			float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
			Vector2 buttonSize = { lineHeight + 2.0f , lineHeight };

			ImGui::SameLine();
			std::string dragID = "##" + _label;
			ImGui::DragFloat(dragID.c_str(), &_value, _speed, _minValue, _maxValue, "%.2f");

			ImGui::PopStyleVar();
			ImGui::EndTable();
		}
	}

	void DrawAxisControl(const String& _label, Float32& _value, Float32 _speed, Float32 _minValue, Float32 _maxValue, Float32 _resetValue, Vector4 _color)
	{
		float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec4 color(_color.x, _color.y, _color.z, _color.w);
		ImVec2 buttonSize = { lineHeight + 2.0f , lineHeight };
		// �� ��ư�� ���� ����
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ color.x - 0.1f, color.y - 0.1f, color.z - 0.1f, 1.0f });

		// ���� ��Ʈ ����
		if (ImGui::Button(_label.c_str(), buttonSize))
		{
			_value = _resetValue;
		}
		ImGui::PopStyleColor(3);

		// �巡�� �����̴�
		ImGui::SameLine();
		std::string dragID = "##" + _label;
		ImGui::DragFloat(dragID.c_str(), &_value, _speed, _minValue, _maxValue, "%.2f");
	}

	void DrawVector3Controller(const String& _label, Vector3& _values, Float32 _resetValue, Float32 _columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;

		ImGui::PushID(_label.c_str());
		if (ImGui::BeginTable(_label.c_str(), 2, flags)) // ���� ID, �� 2��, �÷���
		{
			// ���� �Ӽ��� �����մϴ�. (�ʺ�, ũ�� ���� ��)
			// ImGuiTableColumnFlags_WidthFixed: �ʱ� �ʺ� ����
			// ImGuiTableColumnFlags_WidthStretch: â ũ�⿡ ���� �ʺ� ���� (�⺻��)
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, _columnWidth);
			ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(_label.c_str()); // ���� ��: ���̺�

			ImGui::TableSetColumnIndex(1);
			// [��Ʈ�ѷ� UI �ڵ�] // ������ ��: ��Ʈ�ѷ�
			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2,0 });
			ImGui::PushFont(boldFont);

			DrawAxisControl("X", _values.x, 0.1f, 0.0f, 0.0f, 0.0f, {0.8f, 0.1f,0.1f, 1.0f});

			ImGui::PopItemWidth();

			ImGui::SameLine();
			DrawAxisControl("Y", _values.y, 0.1f, 0.0f, 0.0f, 0.0f, { 0.1f, 0.8f,0.1f, 1.0f });

			ImGui::PopItemWidth();

			ImGui::SameLine();
			DrawAxisControl("Z", _values.z, 0.1f, 0.0f, 0.0f, 0.0f, { 0.1f, 0.1f,0.8f, 1.0f });

			ImGui::PopItemWidth();

			ImGui::PopFont();
			ImGui::PopStyleVar();
			ImGui::EndTable();
		}

		ImGui::Columns(1);

		ImGui::PopID();
	}
	void DrawVector3Controller(const String& _label, Vector3& _values, const char* _Xtag, const char* _Ytag, const char* _Ztag, Float32 _resetValue, Float32 _columnWidth)
	{

	}
	void DrawTransformController(const String& _label, Transform& _transform, Float32 _resetValue, Float32 _columnWidth)
	{
		DrawVector3Controller("Position", _transform.position);
		DrawVector3Controller("Rotation", _transform.rotation);
		DrawVector3Controller("Scale", _transform.scale);
	}
	void DrawLightController(const String& _label, Light& _light)
	{
		ImGui::PushID(_label.c_str());
		const char* lightTypeNames[] = { "Directional", "Point", "Spot" };

		int currentItem = _light.type;

		if (ImGui::Combo("##Light Type", &currentItem, lightTypeNames, IM_ARRAYSIZE(lightTypeNames)))
		{
			// ����ڰ� ��Ӵٿ� �޴����� �ٸ� �׸��� �����ߴٸ�,
			// ����� �ε��� ���� �ٽ� enum Ÿ������ ��ȯ�Ͽ� ���� �����Ϳ� �����մϴ�.
			_light.type = currentItem;
		}

		switch (_light.type)
		{
		case Directional:
			DrawDragFloat("Intensity", _light.intensity, 0.005f, 0.0f, 1.0f);
			DrawColorEditor("Color", _light.color);
			break;
		case Point:
			DrawDragFloat("Intensity", _light.intensity, 0.001f, 0.0f, 1.0f);
			DrawDragFloat("Range", _light.range, 0.1f, 0.0f, 0.0f);
			DrawColorEditor("Color", _light.color);
			break;
		case Spot:
			DrawDragFloat("Intensity", _light.intensity, 0.001f, 0.0f, 1.0f);
			DrawDragFloat("Range", _light.range, 0.1f, 0.0f, 0.0f);
			DrawDragFloat("InnerConeAngle", _light.spotInnerAngle, 0.1f, 0.0f, _light.spotOuterAngle);
			DrawDragFloat("OuterConeAngle", _light.spotOuterAngle, 0.1f, 0.0f, 360.0f);
			DrawColorEditor("Color", _light.color);
			break;
		default:
			break;
		}


		/*DrawColorEditor("Color", _light.color);*/

		ImGui::PopID();
	}
	void DrawModelController(const String& _label, Model& _model)
	{
		ImGui::PushID(_label.c_str());
		for (auto material : _model.GetMaterials())
		{
			ImGui::Text(material->GetTextures().find("Texture")->first.c_str());
			auto texture = material->GetTextures().find("Texture")->second;
			ImGui::Image(texture->GetImGuiHandle(), ImVec2{ 100,100 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE"))
				{
					String texturePath = (const char*)payload->Data;
					auto texture = ResourceManager::GetResource<Texture2D>(texturePath);
					material->SetTexture2D("Texture", texture);
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::PopID();
	}

	void DrawColorEditor(const String& _label, Vector3& _color, Float32 _resetValue, Float32 _columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable;
		ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_Float;

		if (ImGui::BeginTable(_label.c_str(), 2, flags))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, _columnWidth);
			ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(_label.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2,0 });
			ImGui::PushFont(boldFont);
			float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;
			Vector2 buttonSize = { lineHeight + 2.0f , lineHeight };

			DrawAxisControl("R", _color.x, 0.01f, 0.0f, 1.0f, 0.0f, { 0.8f, 0.1f,0.1f, 1.0f });
			ImGui::PopItemWidth();
			ImGui::SameLine();
			DrawAxisControl("G", _color.y, 0.01f, 0.0f, 1.0f, 0.0f, { 0.1f, 0.8f,0.1f, 1.0f });
			ImGui::PopItemWidth();
			ImGui::SameLine();
			DrawAxisControl("B", _color.z, 0.01f, 0.0f, 1.0f, 0.0f,  { 0.1f, 0.1f,0.8f, 1.0f });
			ImGui::PopItemWidth();
			ImGui::SameLine();

			// 1. �ӽ� ���� ���� �� ������ ��ȯ (0~255  ->  0~1)
			ImGui::ColorEdit3("##Color", &_color.r, colorEditFlags);

			ImGui::PopFont();
			ImGui::PopStyleVar();

			ImGui::EndTable();
		}
	}
}
