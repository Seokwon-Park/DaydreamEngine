#pragma once

#include "GameEntity/GameEntity.h"

namespace Daydream
{
	class Scene
	{
	public:
		Scene(const String& _name);
		~Scene();

		GameEntity* CreateGameEntity();

		void Update(Float32 _deltaTime);
	private:
		Array<Unique<GameEntity>> entities;
	};
}
