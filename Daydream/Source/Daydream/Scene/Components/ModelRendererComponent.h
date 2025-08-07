#pragma once

#include "Component.h"
#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class ModelRendererComponent : public Component
	{
	public:
		ModelRendererComponent();
		virtual ~ModelRendererComponent();

		virtual void Init() override;

		void SetModel(Shared<Model> _model);
		void SetMaterial(Shared<Material> _material) { material = _material; }
		void Render();
	private:
		Shared<Model> model;
		Shared<Material> material;
	};
}