#pragma once

#include "GameEntity/GameEntity.h"

namespace Daydream
{
	class ModelRendererComponent;

	class Scene
	{
	public:
		Scene(const String& _name);
		~Scene();

		GameEntity* CreateGameEntity();
		GameEntity* CreateGameEntity(const String& _name);

		Array<ModelRendererComponent*>& GetModelRenderers() { return modelRenderers; }

		void Update(Float32 _deltaTime);
	private:
		Array<Unique<GameEntity>> entities;

		Array<ModelRendererComponent*> modelRenderers;
		//Array<SpriteRendererComponent*> spriteRenderers;
	};
}
