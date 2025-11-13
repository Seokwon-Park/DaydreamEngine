#include "DaydreamPCH.h"
#include "Scene.h"

#include "Components/LightComponent.h"
#include "Components/ModelRendererComponent.h"
#include "Daydream/Graphics/Resources/Skybox.h"

namespace Daydream
{
	Scene::Scene(const String& _name)
	{
		lightBuffer = ConstantBuffer::Create(sizeof(LightData));
		skybox = MakeShared<Skybox>();
		skybox->Init();

		entityPool.push_back(nullptr);
		generations.push_back(0); // 0번 인덱스의 generation은 0
	}

	Scene::~Scene()
	{
	}

	GameEntity* Scene::CreateGameEntity(const String& _name)
	{
		UInt32 index = 0;

		if (!freeIndices.empty()) // 남은 indices가 있으면
		{
			index = freeIndices.front();
			freeIndices.pop();
			entityPool[index] = MakeUnique<GameEntity>();
		}
		else
		{
			index = static_cast<UInt32>(entityPool.size());

			entityPool.push_back(MakeUnique<GameEntity>());
			generations.push_back(0);
		}
		UInt32 generation = generations[index];
		EntityHandle newHandle = EntityHandle::Create(index, generation);

		GameEntity* newEntity = entityPool[index].get();
		newEntity->SetHandle(newHandle);
		newEntity->SetScene(this);
		newEntity->SetName(_name);
		newEntity->Init();

		activeEntities.push_back(newHandle);
		rootEntities.push_back(newHandle);

		return newEntity;
	}

	void Scene::DestroyEntity(EntityHandle _handle)
	{
		if (!IsHandleValid(_handle))
		{
			return; // 이미 파괴되었거나 유효하지 않은 핸들
		}

		UInt32 index = _handle.GetIndex();

		// 1. 엔티티 데이터 리셋 
		entityPool[index]->Reset();

		// 2. 풀에서 엔티티 파괴 (UniquePtr이 nullptr이 됨)
		entityPool[index].reset();

		// 3. Generation 증가 (이전 핸들을 모두 무효화)
		generations[index]++;

		// 4. 인덱스를 재사용 큐에 반환
		freeIndices.push(index);

		// 5. 모든 특수 목록에서 제거
		//removeHandleFromAllLists(_handle);

		// 6. 활성 목록에서 제거 O(N)
		activeEntities.erase(
			std::remove(activeEntities.begin(), activeEntities.end(), _handle),
			activeEntities.end()
		);
	}

	GameEntity* Scene::GetEntity(EntityHandle _handle)
	{
		if (!_handle.IsValid())
		{
			return nullptr;
		}
		return entityPool[_handle.GetIndex()].get();
	}

	const GameEntity* Scene::GetEntity(EntityHandle _handle) const
	{
		if (!_handle.IsValid())
		{
			return nullptr;
		}
		return entityPool[_handle.GetIndex()].get();
	}

	bool Scene::IsHandleValid(EntityHandle _handle) const
	{
		if (!_handle.IsValid()) // ID가 0인지 체크
		{
			return false;
		}

		UInt32 index = _handle.GetIndex();
		if (index >= generations.size()) // 인덱스 범위 체크
		{
			return false;
		}

		// 핸들의 generation과 슬롯의 generation이 일치하는지 확인
		return generations[index] == _handle.GetGeneration();
	}

	void Scene::Update(Float32 _deltaTime)
	{
		for (EntityHandle handle : activeEntities)
		{
			GameEntity* entity = GetEntity(handle);
			if (entity)
			{
				entity->Update(_deltaTime);
			}
		}

		lightData.dirLightCount = 0;
		lightData.pointLightCount = 0;
		lightData.spotLightCount = 0;
		lightData.eyePos = currentCamera->GetPosition();
		for (EntityHandle handle : activeEntities) // 이벤트 기반으로 관리된 목록 사용
		{
			GameEntity* entity = GetEntity(handle);
			if (!entity) continue; // 검사

			ModelRendererComponent* renderComponent = entity->GetComponent<ModelRendererComponent>();
			if (renderComponent != nullptr)
			{
				renderComponent->Render();
			}

			LightComponent* lightComponent = entity->GetComponent<LightComponent>();
			if (lightComponent != nullptr)
			{
				Light light = lightComponent->GetLight();
				switch (light.type)
				{
				case Directional:
				{
					DirectionalLight dirLight;
					dirLight.color = light.color;
					dirLight.direction = light.direction;
					dirLight.intensity = light.intensity;
					lightData.dirLights[lightData.dirLightCount++] = dirLight;
					break;
				}
				case Point:
				{
					PointLight pointLight;
					pointLight.color = light.color;
					pointLight.range = light.range;
					pointLight.intensity = light.intensity;
					pointLight.position = light.position;
					lightData.pointLights[lightData.pointLightCount++] = pointLight;
					break;
				}
				case Spot:
				{
					SpotLight spotLight;
					spotLight.position = light.position;
					spotLight.range = light.range;
					spotLight.direction = light.direction;
					spotLight.intensity = light.intensity;
					spotLight.color = light.color;
					spotLight.innerConeCos = Math::CosDegree(light.spotInnerAngle);
					spotLight.outerConeCos = Math::CosDegree(light.spotOuterAngle);
					lightData.spotLights[lightData.spotLightCount++] = spotLight;
					break;
				}
				}
			}
		}
		lightBuffer->Update(&lightData, sizeof(LightData));

		skybox->Update();
	}

	void Scene::AddRootEntity(EntityHandle _rootEntity)
	{
		rootEntities.push_back(_rootEntity);
	}

	void Scene::RemoveRootEntity(EntityHandle _rootEntity)
	{
		rootEntities.erase(
			std::remove(rootEntities.begin(), rootEntities.end(), _rootEntity),
			rootEntities.end());
	}
	void Scene::ReorderRootEntity(EntityHandle _entityHandle, UInt64 _newIndex)
	{
		// 루트 목록에서 entityHandle을 찾음
		auto it = std::find(rootEntities.begin(), rootEntities.end(), _entityHandle);

		if (it == rootEntities.end())
		{
			// 루트가 아니면 루트로 만들고 순서 지정
			GameEntity* entity = GetEntity(_entityHandle);
			if (entity)
			{
				entity->RemoveParent();
			}
			return;
		}

		// 현재 인덱스 계산
		UInt64 currentIndex = std::distance(rootEntities.begin(), it);

		// 같은 위치면 아무것도 안 함
		if (currentIndex == _newIndex)
		{
			return;
		}

		// 인덱스 범위 체크
		if (_newIndex > rootEntities.size())
		{
			_newIndex = rootEntities.size();
		}

		// 요소를 제거하고 새 위치에 삽입
		EntityHandle movedHandle = *it;
		rootEntities.erase(it);

		// erase 후 삽입 위치 조정
		if (_newIndex > currentIndex)
		{
			_newIndex--;
		}

		rootEntities.insert(rootEntities.begin() + _newIndex, movedHandle);
	}
}