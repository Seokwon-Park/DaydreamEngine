#pragma once

#include "../Components/Component.h"

namespace Daydream
{
	class Component;

	class GameEntity
	{
	public:
		GameEntity();
		~GameEntity();

		inline void SetName(const std::string& _name) { name = _name; }
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

