#include "SteinsPCH.h"
#include "LayerStack.h"

namespace Steins
{
	LayerStack::LayerStack()
	{
		layerInsertIndex = 0;
		//Layers.reserve(512);
	}
	LayerStack::~LayerStack()
	{

	}
	void LayerStack::PushLayer(Layer* _layer)
	{
		layers.emplace(layers.begin() + layerInsertIndex, _layer);
		layerInsertIndex++;
		_layer->OnAttach();
	}
	void LayerStack::PushOverlay(Layer* _overlay)
	{
		layers.emplace_back(_overlay);
		_overlay->OnAttach();
	}
	void LayerStack::PopLayer(Layer* _layer)
	{
		Array<Layer*>::iterator itr = std::find(layers.begin(), layers.begin() + layerInsertIndex + 1, _layer);
		if (itr != layers.end())
		{
			(*itr)->OnDetach();
			delete* itr;
			layers.erase(itr);
			layerInsertIndex--;
		}
		else
		{
			STEINS_CORE_WARN("{0} is Not Found In Layers", _layer->GetName());
		}
	}
	void LayerStack::PopOverlay(Layer* _overlay)
	{
		Array<Layer*>::iterator itr = std::find(layers.begin()+ layerInsertIndex, layers.end(), _overlay);
		if (itr != layers.end())
		{
			(*itr)->OnDetach();
			delete* itr;
			layers.erase(itr);
		}
		else
		{
			STEINS_CORE_WARN("{0} is Not Found In Overlays", _overlay->GetName());
		}
	}
	void LayerStack::Release()
	{
		for (Layer* layer : layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}
}