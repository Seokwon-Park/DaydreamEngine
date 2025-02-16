#pragma once

#include "Layer.h"

#include <vector>

namespace Steins
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);
		void PopLayer(Layer* _layer);
		void PopOverlay(Layer* _overlay);

		std::vector<Layer*>::iterator begin() { return Layers.begin(); }
		std::vector<Layer*>::iterator end() { return Layers.end(); }
	private:
		std::vector<Layer*> Layers;
		uint32 LayerInsertIndex;
	};
}
