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
		lightComponents.push_back(_light);
	}

	void Scene::AddModelRenderer(ModelRendererComponent* _modelRenderer)
	{
		modelRenderers.push_back(_modelRenderer);
	}

	void Scene::Update(Float32 _deltaTime)
	{
		//lightData.dirLightCount = 0;
		//lightData.pointLightCount = 0;
		//lightData.spotLightCount = 0;
		//lightData.eyePos = currentCamera->GetPosition();
		//for (auto lightComponent : lightComponents)
		//{
		//	Light light = lightComponent->GetLight();
		//	switch (light.type)
		//	{
		//	case Directional:
		//	{
		//		DirectionalLight dirLight;
		//		dirLight.color = light.color;
		//		dirLight.direction = light.direction;
		//		dirLight.intensity = light.intensity;
		//		lightData.dirLights[lightData.dirLightCount++] = dirLight;
		//		break;
		//	}
		//	case Point:
		//	{
		//		PointLight pointLight;
		//		pointLight.color = light.color;
		//		pointLight.intensity = light.intensity;
		//		pointLight.position = light.position;
		//		lightData.pointLights[lightData.pointLightCount++] = pointLight;
		//		break;
		//	}
		//	case Spot:
		//	{
		//		SpotLight spotLight;
		//		spotLight.color = light.color;
		//		spotLight.direction = light.direction;
		//		spotLight.innerConeCos = light.spotInnerAngle;
		//		spotLight.intensity = light.spotInnerAngle;
		//		spotLight.outerConeCos = light.spotOuterAngle;
		//		spotLight.position = light.position;
		//		spotLight.range = light.range;
		//		lightData.spotLights[lightData.spotLightCount++] = spotLight;
		//		break;
		//	}
		//	}
		//	lightBuffer->Update(&lightData, sizeof(LightData));
		//}

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