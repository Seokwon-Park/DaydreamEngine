#pragma once

#include <chrono>

namespace Steins
{
	class TimeStep
	{
	public:
		TimeStep(float _fixedDeltaTime = 1.0f / 60.0f);


		void UpdateTime();
		inline float GetDeltaTime() const { return deltaTime; };
	private:
		float deltaTime;
		float fixedDeltaTime;

		std::chrono::high_resolution_clock::time_point lastTime;

	};
}