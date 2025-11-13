#include "DaydreamPCH.h"
#include "GameEntity.h"
#include "Daydream/Scene/Components/TransformComponent.h"
#include "Daydream/Scene/Scene.h"

namespace Daydream
{
	GameEntity::GameEntity()
	{
	}

	GameEntity::~GameEntity()
	{
	}

	void GameEntity::Init()
	{
		AddComponent<TransformComponent>();
	}

	void GameEntity::Update(Float32 _deltaTime)
	{
		//DAYDREAM_CORE_TRACE("{0} is Updated", name);
		for (auto& component : components)
		{
			component->Update(_deltaTime);
		}
	}
	GameEntity* GameEntity::GetParent()
	{
		if (!scene) return nullptr;

		return scene->GetEntity(parentHandle);
	}

	void GameEntity::SetParent(EntityHandle _parentHandle)
	{
		if (!scene)
		{
			DAYDREAM_CORE_ERROR("Entity has no scene!");
			return;
		}

		if (handle == _parentHandle) // 부모가 자기 자신이 될 수 없음
		{
			DAYDREAM_CORE_ERROR("Cannot set self as parent!");
			return;
		}

		if (parentHandle == _parentHandle) // 새로운 부모가 원래 부모와 같다면 바꿀필요가 없음
		{
			return;
		}

		GameEntity* newParent = nullptr;
		if (_parentHandle.IsValid()) // 새로운 부모의 핸들이 유효한지 확인
		{
			newParent = scene->GetEntity(_parentHandle);
			if (!newParent) // 새로운 부모의 핸들로 가져온 포인터가 유효한지 확인
			{
				DAYDREAM_CORE_ERROR("Invalid parent handle - entity not found!");
				return; // 실패하면 아무것도 변경하지 않음
			}

			// 순환 참조 체크 (새 부모가 this의 자손인지)
			GameEntity* checkParent = newParent;
			while (checkParent)
			{
				if (checkParent == this) // 포인터 직접 비교
				{
					DAYDREAM_CORE_ERROR("Cannot set descendant as parent - circular reference!");
					return;
				}

				EntityHandle nextParentHandle = checkParent->GetParentHandle();
				if (!nextParentHandle.IsValid())
				{
					break; // 루트 도달
				}

				checkParent = scene->GetEntity(nextParentHandle);
				if (!checkParent) // nullptr 체크
				{
					DAYDREAM_CORE_WARN("Parent chain broken - entity not found");
					break;
				}
			}
		}

		if (parentHandle.IsValid())
		{
			GameEntity* oldParent = scene->GetEntity(parentHandle);
			if (oldParent)
			{
				oldParent->RemoveChild(handle); // oldParent의 자식 목록에서 'this' 제거
			}
		}
		else
		{
			// 기존 부모가 없었다면, 씬의 루트 엔티티였음
			scene->RemoveRootEntity(handle); // 씬의 루트 목록에서 'this' 제거
		}

		// 5. 'this'의 부모 핸들을 새 핸들로 갱신
		parentHandle = _parentHandle;

		// 6. 새 부모에 'this'를 자식으로 추가
		if (newParent) // _parentHandle이 유효한 경우
		{
			newParent->AddChild(handle); // newParent의 자식 목록에 'this' 추가
		}
		else
		{
			// _parentHandle이 유효하지 않은 경우(nullptr), 'this'는 루트 엔티티가 됨
			scene->AddRootEntity(handle); // 씬의 루트 목록에 'this' 추가
		}
	}

	void GameEntity::RemoveParent()
	{
		if (!scene)
		{
			DAYDREAM_CORE_ERROR("Entity has no scene!");
			return;
		}

		// 이미 루트 엔티티인 경우
		if (!parentHandle.IsValid())
		{
			DAYDREAM_CORE_WARN("Entity already has no parent.");
			return;
		}

		// 기존 부모에서 제거
		GameEntity* oldParent = scene->GetEntity(parentHandle);
		if (oldParent)
		{
			oldParent->RemoveChild(handle);
		}
		else
		{
			DAYDREAM_CORE_WARN("Parent entity not found.");
		}

		// 부모 핸들 무효화
		parentHandle = EntityHandle();

		// 루트 엔티티로 등록
		scene->AddRootEntity(handle);
	}

	void GameEntity::AddChild(EntityHandle _childHandle)
	{
		if (!scene)
		{
			DAYDREAM_CORE_ERROR("Entity has no scene!");
			return;
		}

		if (handle == _childHandle)
		{
			DAYDREAM_CORE_WARN("An entity cannot be its own child.");
			return;
		}

		if (!_childHandle.IsValid())
		{
			DAYDREAM_CORE_WARN("Invalid child handle.");
			return;
		}

		auto itr = std::find(childrenHandles.begin(), childrenHandles.end(), _childHandle);
		if (itr != childrenHandles.end())
		{
			return;
		}

		GameEntity* newChild = scene->GetEntity(_childHandle);

		// 자식 엔티티 포인터가 유효한지 확인
		if (!newChild)
		{
			// 핸들은 유효(IsValid)했지만 Scene에서 엔티티를 찾지 못함 (예: 이미 파괴됨)
			DAYDREAM_CORE_ERROR("Child entity not found in scene!");
			return;
		}

		childrenHandles.push_back(_childHandle);
	}

	void GameEntity::RemoveChild(EntityHandle _childHandle)
	{
		// C++ 표준 라이브러리의 'erase-remove idiom' 
		// _childHandle이 아닌 값들을 앞쪽으로 shift 삭제할 값이 시작되는 위치를 return 
		// 모여 있는 삭제될 값들을 한번에 삭제

		childrenHandles.erase(
			std::remove(childrenHandles.begin(), childrenHandles.end(), _childHandle),
			childrenHandles.end()
		);
	}

	void GameEntity::ReorderChild(EntityHandle childHandle, UInt64 newIndex)
	{
		if (!scene)
		{
			DAYDREAM_CORE_ERROR("Entity has no scene!");
			return;
		}

		// 자식 목록에서 childHandle을 찾음
		auto it = std::find(childrenHandles.begin(), childrenHandles.end(), childHandle);

		if (it == childrenHandles.end())
		{
			// 자식이 아니면 추가 후 순서 조정
			GameEntity* child = scene->GetEntity(childHandle);
			if (child)
			{
				child->SetParent(handle);
			}
			return;
		}

		// 현재 인덱스 계산
		UInt64 currentIndex = std::distance(childrenHandles.begin(), it);

		// 같은 위치면 아무것도 안 함
		if (currentIndex == newIndex)
		{
			return;
		}

		// 인덱스 범위 체크
		if (newIndex > childrenHandles.size())
		{
			newIndex = childrenHandles.size();
		}

		// 요소를 제거하고 새 위치에 삽입
		EntityHandle movedHandle = *it;
		childrenHandles.erase(it);

		// erase 후 삽입 위치 조정
		if (newIndex > currentIndex)
		{
			newIndex--;
		}

		childrenHandles.insert(childrenHandles.begin() + newIndex, movedHandle);
	}
}
