#include "DaydreamPCH.h"
#include "GameEntity.h"
#include "Daydream/Scene/Components/TransformComponent.h"

namespace Daydream
{
	GameEntity::GameEntity()
	{
		AddComponent<TransformComponent>();
	}

	GameEntity::~GameEntity()
	{
	}

	void GameEntity::Update(Float32 _deltaTime)
	{
		//DAYDREAM_CORE_TRACE("{0} is Updated", name);
		for (auto& component : components)
		{
			component->Update(_deltaTime);
		}
	}
}
