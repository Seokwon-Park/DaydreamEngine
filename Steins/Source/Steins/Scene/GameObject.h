#pragma once

namespace Steins
{
	class Component;

	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		inline void SetName(const std::string& _name) { name = _name; }

		template <class Component, typename ...Args>
		void AddComponent(Args& ...args);
	protected:

	private:
		std::vector<Shared<Component>> components;

		GameObject* parent;
		std::vector<GameObject*> children;

		std::string name;
	};
}

