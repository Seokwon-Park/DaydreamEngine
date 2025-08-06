#pragma once

#include "Component.h"
#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelRendererComponent : public Component
	{
	public:
		ModelRendererComponent();
		virtual ~ModelRendererComponent() {};

		void SetModel(Shared<Model> _model) { model = _model; };
		void Render();
	private:
		Shared<Model> model;
		Shared<Material> material;
	};
}