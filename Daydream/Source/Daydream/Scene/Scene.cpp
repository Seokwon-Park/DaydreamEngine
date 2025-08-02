#include "DaydreamPCH.h"
#include "Scene.h"

namespace Daydream
{
	Scene::Scene(const std::string& _name)
	{

	}

	Scene::~Scene()
	{
	}

	GameEntity* Scene::CreateGameEntity()
	{
		Unique<GameEntity> entity = MakeUnique<GameEntity>();
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