#pragma once

#include "Steins/Core/Layer.h"



namespace Steins
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& _event) override;
		
		void BeginImGui();
		void EndImGui();

		void BlockEvents(bool _isBlock) { blockEvents = _isBlock; }
	protected:

	private:
		void SetDarkThemeColors();

		bool blockEvents = true;
	};
}
