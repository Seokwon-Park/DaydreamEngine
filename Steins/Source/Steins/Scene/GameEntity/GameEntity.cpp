#include "SteinsPCH.h"
#include "GameEntity.h"

namespace Steins
{
	GameEntity::GameEntity()
	{
	}

	GameEntity::~GameEntity()
	{
		for (auto component : components)
		{
			delete component;
		}
	}
}
