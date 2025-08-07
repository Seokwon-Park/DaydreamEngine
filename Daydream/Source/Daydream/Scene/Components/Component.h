#pragma once

namespace Daydream
{
	class GameEntity;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		inline void SetOwner(GameEntity* _entity) { owner = _entity; }
		inline GameEntity* GetOwner() const { return owner; }

		virtual void Init();
		virtual void Update(Float32 _deltaTime);
	protected:
		GameEntity* owner = nullptr;
	};
}