#pragma once

namespace Steins
{
	class Component;

	class GameEntity
	{
	public:
		GameEntity();
		~GameEntity();

		inline void SetName(const std::string& _name) { name = _name; }

		template <class Component, typename ...Args>
		void AddComponent(Args& ...args);
	protected:

	private:
		Array<Shared<Component>> components;

		GameEntity* parent;
		Array<GameEntity*> children;

		std::string name;

	};
}

