#include "DaydreamPCH.h"
#include "GameEntity.h"

namespace Daydream
{
	GameEntity::GameEntity()
	{
	}

	GameEntity::~GameEntity()
	{
	}

	void GameEntity::Update(Float32 _deltaTime)
	{
		DAYDREAM_CORE_TRACE("{0} is Updated", name);
	}
}
