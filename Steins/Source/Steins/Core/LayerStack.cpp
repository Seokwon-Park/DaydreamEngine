#include "SteinsPCH.h"
#include "LayerStack.h"

namespace Steins
{
	LayerStack::LayerStack()
	{
		LayerInsertIndex = 0;
		//Layers.reserve(512);
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* _layer)
	{
		Layers.emplace(Layers.begin() + LayerInsertIndex, _layer);
		LayerInsertIndex++;
	}
	void LayerStack::PushOverlay(Layer* _overlay)
	{
		Layers.emplace_back(_overlay);
	}
	void LayerStack::PopLayer(Layer* _layer)
	{
		std::vector<Layer*>::iterator itr = std::find(Layers.begin(), Layers.begin() + LayerInsertIndex + 1, _layer);
		if (itr != Layers.end())
		{
			delete* itr;
			Layers.erase(itr);
			LayerInsertIndex--;
		}
		else
		{
			STEINS_CORE_WARN("{0} is Not Found In Layers", _layer->GetName());
		}
	}
	void LayerStack::PopOverlay(Layer* _overlay)
	{
		std::vector<Layer*>::iterator itr = std::find(Layers.begin()+LayerInsertIndex, Layers.end(), _overlay);
		if (itr != Layers.end())
		{
			Layers.erase(itr);
		}
		else
		{
			STEINS_CORE_WARN("{0} is Not Found In Overlays", _overlay->GetName());
		}
	}
}