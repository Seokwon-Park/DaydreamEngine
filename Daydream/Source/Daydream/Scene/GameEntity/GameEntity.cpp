#include "DaydreamPCH.h"
#include "GameEntity.h"

namespace Daydream
{
	GameEntity::GameEntity()
	{
	}

	GameEntity::~GameEntity()
	{
		for (auto& component : components)
		{
			delete component;
		}
	}

	void GameEntity::Update(Float32 _deltaTime)
	{
		//DAYDREAM_CORE_TRACE("{0} is Updated", name);
		for (auto& component : components)
		{

		}
	}
}
