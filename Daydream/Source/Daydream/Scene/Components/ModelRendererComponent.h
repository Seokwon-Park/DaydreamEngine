#pragma once

#include "Component.h"
#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	struct WorldCosntantBuffer
	{
		Matrix4x4 world;
		Matrix4x4 invTranspose;
	};

	class ModelRendererComponent : public Component
	{
	public:
		DEFINE_COMPONENT_NAME(ModelRendererComponent);

		ModelRendererComponent();
		virtual ~ModelRendererComponent();

		virtual void Init() override;
		virtual void Update(Float32 _deltaTime) override {};

		void SetModel(Shared<Model> _model);
		void SetMaterial(Shared<Material> _material) { material = _material.get(); }
		void Render();

		REFLECT_START()
			ADD_PROPERTY(FieldType::Model, model)
			ADD_PROPERTY(FieldType::Material, material)
		REFLECT_END()
	private:
		Model* model;
		Material* material;
		Shared<ConstantBuffer> worldMatrix;
	};
}