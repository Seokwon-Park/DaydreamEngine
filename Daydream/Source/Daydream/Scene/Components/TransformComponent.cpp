#include "DaydreamPCH.h"
#include "TransformComponent.h"

#include "Daydream/Scene/GameEntity/GameEntity.h"

namespace Daydream
{
	TransformComponent::TransformComponent()
	{
	}
	TransformComponent::~TransformComponent()
	{
	}
	void TransformComponent::Init()
	{
	}

	void TransformComponent::Update(Float32 _deltaTime)
	{
	}

	Matrix4x4 TransformComponent::GetWorldMatrix()
	{
		Matrix4x4 localMat = transform.GetLocalMatrix();

		GameEntity* owner = GetOwner();
		if (owner)
		{
			GameEntity* parent = owner->GetParent();
			if (parent)
			{
				auto parentTransform = parent->GetComponent<TransformComponent>();

				return localMat * parentTransform->GetWorldMatrix();
			}
		}
		return localMat;
	}

	void TransformComponent::SetTransform(Transform _transform)
	{
		transform = _transform;
	}

	void TransformComponent::SetTransform(Vector3 _position, Vector3 _rotation, Vector3 _scale)
	{
		transform.position = _position;
		transform.rotation = _rotation;
		transform.scale = _scale;
	}

}
