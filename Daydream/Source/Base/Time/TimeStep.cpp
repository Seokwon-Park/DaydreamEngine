#include "DaydreamPCH.h"
#include "TimeStep.h"

namespace Daydream
{
	TimeStep::TimeStep(float _fixedDeltaTime)
		:fixedDeltaTime(_fixedDeltaTime), deltaTime(0.0f), lastTime(std::chrono::high_resolution_clock::now())
	{

	}

	void TimeStep::UpdateTime()
	{
		auto now = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(now - lastTime).count();
		lastTime = now;

	}
}
