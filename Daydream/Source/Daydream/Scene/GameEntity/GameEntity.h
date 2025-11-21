#pragma once

#include "Daydream/Scene/Components/Component.h"
#include "Daydream/Scene/Components/ModelRendererComponent.h"
#include "EntityHandle.h"

namespace Daydream
{
	class Scene;
	class Component;

	class GameEntity
	{
	public:
		GameEntity();
		~GameEntity();

		inline const String& GetName() const { return name; }
		inline void SetName(const String& _name) { name = _name; }

		Scene* GetScene() { return scene; }
		inline void SetScene(Scene* _scene) { scene = _scene; }

		/// 자신의 핸들을 설정하고 가져오는 함수
		const EntityHandle& GetHandle() const { return handle; }
		void SetHandle(EntityHandle _handle) { handle = _handle; }

		bool IsValid() const { return handle.IsValid(); }

		void Init();
		void Update(Float32 _deltaTime);

		EntityHandle GetParentHandle() const { return parentHandle; }
		GameEntity* GetParent();
		void SetParent(EntityHandle _parentHandle);
		void RemoveParent();

		// 자식 핸들 목록을 반환
		const Array<EntityHandle>& GetChildrenHandles() const { return childrenHandles; }


		template <class ComponentType>
		ComponentType* GetComponent()
		{
			auto it = componentMap.find(std::type_index(typeid(ComponentType)));
			if (it == componentMap.end())
			{
				return nullptr; // 해당 타입의 컴포넌트가 없음
			}

			return static_cast<ComponentType*>(it->second);
		}

		template <class ComponentType>
		ComponentType* AddComponent()
		{
			static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");
			Unique<ComponentType> newComponent = MakeUnique<ComponentType>();
			newComponent->SetOwner(this);
			newComponent->Init();
			ComponentType* rawPtr = newComponent.get();
			std::type_index id = std::type_index(typeid(ComponentType));
			if (componentMap.find(id) != componentMap.end())
			{
				return nullptr;
			}
			componentMap[id] = rawPtr;
			components.push_back(std::move(newComponent));
			return rawPtr;
		};
	
		template <class ComponentType>
		bool HasComponent()
		{
			auto itr = componentMap.find(std::type_index(typeid(ComponentType)));
			return itr != componentMap.end();
		}

		Array<Unique<Component>>& GetAllComponents() { return components; };

		void Reset()
		{
			components.clear();
			componentMap.clear();
			scene = nullptr;
			parentHandle = EntityHandle(); // 유효하지 않은 핸들로 초기화
			childrenHandles.clear();
			name = "";
			handle = EntityHandle(); // 자기 자신 핸들도 초기화
		}

		void ReorderChild(EntityHandle _childHandle, UInt64 _newIndex);
	
	protected:

	private:
		// 자식 추가/제거
		void AddChild(EntityHandle _childHandle);
		void RemoveChild(EntityHandle _childHandle);

		Array<Unique<Component>> components;
		HashMap<std::type_index, Component*> componentMap;

		Scene* scene;
		EntityHandle handle; 

		EntityHandle parentHandle;
		Array<EntityHandle> childrenHandles;

		String name;
	};
}

