#pragma once

#include "Daydream/Event/Event.h"

namespace Daydream
{
	class Layer
	{
	public:
		Layer(const std::string& _name = "LayerName");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Float32 _deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& _event) {}

		inline const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;
	};
}

