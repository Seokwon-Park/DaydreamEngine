#pragma once

#include "Layer.h"

#include <vector>

namespace Daydream
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

		Array<Layer*>::iterator begin() { return layers.begin(); }
		Array<Layer*>::iterator end() { return layers.end(); }
	private:
		Array<Layer*> layers;
		UInt32 layerInsertIndex;
	};
}
