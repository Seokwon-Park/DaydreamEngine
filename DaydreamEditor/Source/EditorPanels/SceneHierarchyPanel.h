#pragma once

#include "Daydream.h"
#include "UIPanel.h"


namespace Daydream
{
	class SceneHierarchyPanel : public UIPanel
	{
	public:
		SceneHierarchyPanel();

		void SetCurrentScene(Scene* _scene);
		GameEntity* GetSelectedEntity() { return selectedEntity; }

		void OnImGuiRender();
	private:
		GameEntity* selectedEntity = nullptr;
		Scene* scene = nullptr;
		char searchQuery[256] = "";
	};
}