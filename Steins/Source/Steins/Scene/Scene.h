#pragma once

#include "GameEntity/GameEntity.h"

namespace Steins
{
	class Scene
	{
	public:
		Scene(const String& _name);
		~Scene();

		GameEntity* CreateGameEntity();
	private:

		Array<GameEntity*> entities;
	};
}
