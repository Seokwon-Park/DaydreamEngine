#pragma once

#include "Daydream/Scene/Components/Component.h"
#include "Daydream/Scene/Components/ModelRendererComponent.h"

namespace Daydream
{
	class Scene;
	class Component;

	class GameEntity
	{
	public:
		GameEntity();
		~GameEntity();

		inline void SetName(const String& _name) { name = _name; }
		inline const String& GetName() { return name; }
		inline void SetScene(Scene* _scene) { scene = _scene; }
		Scene* GetScene() { return scene; }
		void Init();
		void Update(Float32 _deltaTime);

		void SetParent(GameEntity* _parent)
		{
			parent = _parent;
		}

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
			return itr == componentMap.end();
		}

		Array<Unique<Component>>& GetAllComponents() { return components; };
	protected:

	private:
		Array<Unique<Component>> components;
		HashMap<std::type_index, Component*> componentMap;

		Scene* scene;

		GameEntity* parent = nullptr;
		Array<GameEntity*> childrens;

		std::string name;
	};
}

