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

		void Release();

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
	private:
		std::vector<Layer*> layers;
		UInt32 layerInsertIndex;
	};
}
