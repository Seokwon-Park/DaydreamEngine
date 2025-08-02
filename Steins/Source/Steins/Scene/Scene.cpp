#include "SteinsPCH.h"
#include "Scene.h"

namespace Steins
{
	Scene::Scene(const std::string& _name)
	{

	}

	Scene::~Scene()
	{
		for (auto entity : entities)
		{
			delete entity;
		}
	}

	GameEntity* Scene::CreateGameEntity()
	{
		GameEntity* entity = new GameEntity();

		entities.push_back(entity);

		return entity;
	}
}