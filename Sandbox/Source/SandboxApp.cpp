#include <Steins.h>

class Sandbox : public Steins::Application
{
public:
	Sandbox()
		:Steins::Application()
	{
	}

	~Sandbox()
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	return new Sandbox();
}