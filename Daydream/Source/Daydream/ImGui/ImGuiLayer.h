#pragma once

#include "ImGuiRenderer.h"

#include "Daydream/Core/Layer.h"

#include "Daydream/Event/KeyEvent.h"
#include "Daydream/Event/MouseEvent.h"
#include "Daydream/Event/ApplicationEvent.h"


namespace Daydream
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		//Init ImGui For _window
		void Init();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& _event) override;
		
		void BeginImGui();
		void EndImGui();

		void BlockEvents(bool _isBlockEvents) { isBlockEvents = _isBlockEvents; }
	protected:

	private:
		void SetDarkThemeColors();

		Unique<ImGuiRenderer> renderer;
		
		bool isBlockEvents = true;
	};
}
