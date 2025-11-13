#pragma once

namespace Daydream
{
	class Timestep
	{
	public:
		Timestep(float _time = 0.0f)
			:floatTime(_time)
		{

		}

		operator float() const { return floatTime; }

		float GetSeconds() const { return floatTime; }
		float GetMilliseconds() const { return floatTime * 1000.0f; }
	private:
		float floatTime;
	};
}