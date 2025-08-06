#pragma once

#include "Daydream/Scene/Components/Component.h"

namespace Daydream
{
	class Component;

	class GameEntity : public std::enable_shared_from_this<GameEntity>
	{
	public:
		GameEntity();
		~GameEntity();

		inline void SetName(const String& _name) { name = _name; }
		void Update(Float32 _deltaTime);

		void SetParent(GameEntity* _parent)
		{
			parent = _parent;
		}

		template <class ComponentType>
		ComponentType AddComponent()
		{
			Unique<ComponentType> newComponent = MakeUnique<ComponentType>();
			static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");
			Component* temp = static_cast<Component*>(newComponent.get());
			temp->SetOwner(this);
			components.emplace_back(newComponent);
			return newComponent.get();
		};
	protected:

	private:
		Array<Unique<Component>> components;

		GameEntity* parent = nullptr;
		Array<GameEntity*> children;

		std::string name;
	};
}

