#pragma once

#include "Component.h"

namespace Daydream
{
	class TransformComponent : public Component
	{
	public:
		DEFINE_COMPONENT_NAME(TransformComponent);

		TransformComponent();
		virtual ~TransformComponent();

		virtual void Init() override;
		virtual void Update(Float32 _deltaTime) override;

		Transform& GetTransform() { return transform; }

		REFLECT_START()
			ADD_PROPERTY(FieldType::Transform, transform)
		REFLECT_END()
	private:
		Transform transform = Transform();
	};
}