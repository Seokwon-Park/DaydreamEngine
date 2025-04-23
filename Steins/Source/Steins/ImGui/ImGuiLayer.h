#pragma once

#include "ImGuiRenderer.h"

#include "Steins/Core/Layer.h"

#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/MouseEvent.h"
#include "Steins/Event/ApplicationEvent.h"


namespace Steins
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
