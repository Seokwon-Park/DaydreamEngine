#pragma once

#include "Steins/Event/Event.h"

namespace Steins
{
	class Layer
	{
	public:
		Layer(const std::string& _name = "LayerName");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& _event) {}

		inline const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;
	};
}

