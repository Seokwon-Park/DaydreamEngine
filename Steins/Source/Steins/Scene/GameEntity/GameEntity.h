#pragma once

#include "../Components/Component.h"

namespace Steins
{
	class Component;

	class GameEntity
	{
	public:
		GameEntity();
		~GameEntity();

		inline void SetName(const std::string& _name) { name = _name; }

		template <class ComponentType>
		ComponentType AddComponent()
		{
			ComponentType* newComponent = new ComponentType();

			static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");

			components.push_back(newComponent);
		};
	protected:

	private:
		Array<Component*> components;

		GameEntity* parent;
		Array<GameEntity*> children;

		std::string name;
	};
}

