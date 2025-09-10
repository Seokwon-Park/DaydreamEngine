#pragma once

#include "GameEntity/GameEntity.h"

#include "Daydream/Graphics/Core/Camera.h"
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

		inline const Array<LightComponent*>& GetLights() { return lights; }
		inline const Array<ModelRendererComponent*>& GetModelRenderers() { return modelRenderers; }

		inline void SetCurrentCamera(Shared<Camera> _camera) { currentCamera = _camera; }
		inline const Shared<Camera> GetCurrentCamera() const { return currentCamera; }

		inline Shared<ConstantBuffer> GetLightConstantBuffer() const { return lightBuffer; }

		void Update(Float32 _deltaTime);

		Array<Unique<GameEntity>>& GetAllEntities() { return entities; }
	private:
		Array<Unique<GameEntity>> entities;

		Shared<Camera> currentCamera;

		Array<LightComponent*> lights;
		Array<ModelRendererComponent*> modelRenderers;

		LightData lightData;
		Shared<ConstantBuffer> lightBuffer;
		//Array<SpriteRendererComponent*> spriteRenderers;
	};
}
