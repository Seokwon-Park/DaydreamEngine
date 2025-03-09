#pragma once

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
		static void Init(class SteinsWindow* _window);

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& _event) override;
		
		void BeginImGui();
		void EndImGui();

		void BlockEvents(bool _isBlockEvents) { isBlockEvents = _isBlockEvents; }
	protected:

	private:
		void SetDarkThemeColors();

		bool isBlockEvents = true;
	};
}
