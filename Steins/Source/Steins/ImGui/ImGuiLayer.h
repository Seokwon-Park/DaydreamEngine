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
		void OnUpdate() override;
		void OnEvent(Event& _event) override;

	protected:

	private:

	};
}
