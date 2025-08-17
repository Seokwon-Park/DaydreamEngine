#pragma once

#include "GameEntity/GameEntity.h"

#include "Daydream/Graphics/Resources/Light.h"

namespace Daydream
{
	class ModelRendererComponent;
	class LightComponent;

	struct LightData
	{
		Light lights[32]; // 최대 32개 라이트
		Vector3 eyePos;
		int lightCount;
	};

	class Scene
	{
	public:
		Scene(const String& _name);
		~Scene();

		GameEntity* CreateGameEntity();
		GameEntity* CreateGameEntity(const String& _name);

		void AddLight(LightComponent* _light);
		void AddModelRenderer(ModelRendererComponent* _modelRenderer);

		const Array<LightComponent*>& GetLights() { return lights; }
		const Array<ModelRendererComponent*>& GetModelRenderers() { return modelRenderers; }

		void Update(Float32 _deltaTime);

		Array<Unique<GameEntity>>& GetAllEntities() { return entities; }
	private:
		Array<Unique<GameEntity>> entities;

		Array<LightComponent*> lights;
		Array<ModelRendererComponent*> modelRenderers;
		//Array<SpriteRendererComponent*> spriteRenderers;
	};
}
