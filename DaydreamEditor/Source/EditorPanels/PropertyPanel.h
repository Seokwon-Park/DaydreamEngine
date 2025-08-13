#pragma once

#include "Daydream.h"
#include "UIPanel.h"

namespace Daydream
{
	class PropertyPanel : public UIPanel
	{
	public:
		PropertyPanel();

		virtual void OnImGuiRender() override;

		inline void SetSelectedEntity(GameEntity* _selectedEntity) { selectedEntity = _selectedEntity; }
		void DrawComponentProperties(Component* _component);

	private:
		GameEntity* selectedEntity = nullptr;
	};
}