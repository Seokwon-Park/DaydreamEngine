#include "DaydreamPCH.h"
#include "Scene.h"

#include "Components/LightComponent.h"
#include "Components/ModelRendererComponent.h"

namespace Daydream
{
	Scene::Scene(const String& _name)
	{
		lightBuffer = ConstantBuffer::Create(sizeof(LightData));
	}

	Scene::~Scene()
	{
	}

	GameEntity* Scene::CreateGameEntity()
	{
		return CreateGameEntity("Empty Entity");
	}

	GameEntity* Scene::CreateGameEntity(const String& _name)
	{
		Unique<GameEntity> newEntity = MakeUnique<GameEntity>();
		newEntity->SetName(_name);
		newEntity->SetScene(this);
		GameEntity* rawPtr = newEntity.get();
		entities.push_back(std::move(newEntity));
		return rawPtr;
	}

	void Scene::AddLight(LightComponent* _light)
	{
		lights.push_back(_light);
	}

	void Scene::AddModelRenderer(ModelRendererComponent* _modelRenderer)
	{
		modelRenderers.push_back(_modelRenderer);
	}

	void Scene::Update(Float32 _deltaTime)
	{
		lightData.lightCount = 0;
		lightData.eyePos = currentCamera->GetPosition();
		for (auto lightComponent : lights)
		{
			lightData.lights[lightData.lightCount++] = lightComponent->GetLight();
		}
		lightBuffer->Update(&lightData, sizeof(LightData));

		for (const auto& entity : entities)
		{
			entity->Update(_deltaTime);
		}

		for (const auto& renderer : modelRenderers)
		{
			renderer->Render();
		}
	}
}