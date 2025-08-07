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
		inline void SetScene(Scene* _scene) { scene = _scene; }
		Scene* GetScene() { return scene; }
		void Init();
		void Update(Float32 _deltaTime);

		void SetParent(GameEntity* _parent)
		{
			parent = _parent;
		}

		template <class ComponentType>
		ComponentType* AddComponent()
		{
			ComponentType* newComponent = new ComponentType();
			static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");
			newComponent->SetOwner(this);
			newComponent->Init();
			Component* temp = static_cast<Component*>(newComponent);
			components.push_back(newComponent);
			return newComponent;
		};
	protected:

	private:
		Array<Component*> components;

		Scene* scene;

		GameEntity* parent = nullptr;
		Array<GameEntity*> children;

		std::string name;
	};
}

