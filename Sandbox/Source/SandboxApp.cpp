#include <Steins.h>

class Sandbox : public Steins::Application
{
public:
	Sandbox()
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