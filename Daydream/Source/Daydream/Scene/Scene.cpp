#include "DaydreamPCH.h"
#include "Scene.h"

#include "Components/ModelRendererComponent.h"

namespace Daydream
{
	Scene::Scene(const String& _name)
	{
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
		Unique<GameEntity> entity = MakeUnique<GameEntity>();
		entity->SetName(_name);
		GameEntity* rawPtr = entity.get();
		entities.emplace_back(std::move(entity));
		return rawPtr;
	}

	void Scene::Update(Float32 _deltaTime)
	{
		for (const auto& entity : entities)
		{
			entity->Update(_deltaTime);
		}
	}
}