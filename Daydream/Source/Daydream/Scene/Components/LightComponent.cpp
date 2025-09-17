#include "DaydreamPCH.h"
#include "LightComponent.h"

#include "Daydream/Scene/Scene.h"
#include "TransformComponent.h"

namespace Daydream
{
	LightComponent::LightComponent()
	{
	}
	LightComponent::~LightComponent()
	{
	}
	void LightComponent::Init()
	{
		GetOwner()->GetScene()->AddLight(this);
	}
	void LightComponent::Update(Float32 _deltaTime)
	{
		Transform transform = GetOwner()->GetComponent<TransformComponent>()->GetTransform();
		light.direction = transform.GetForward();
		light.position = transform.position;

	}
}