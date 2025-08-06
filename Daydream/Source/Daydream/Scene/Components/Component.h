#pragma once

namespace Daydream
{
	class GameEntity;

	class Component
	{
	public:
		Component();

		inline void SetOwner(GameEntity* _entity) { owner = _entity; }
		inline GameEntity* GetOwner() const { return owner; }
	private:
		GameEntity* owner = nullptr;
	};
}