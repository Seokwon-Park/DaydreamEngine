#include <Steins.h>

class ExampleLayer : public Steins::Layer 
{
public :
	ExampleLayer()
		:Layer("Test") {}

	void OnUpdate() override
	{

		Steins::Color c;
		c.x = 10;

		//STEINS_INFO("TestLayer::UPDATE");
		/*if(Steins::Input::GetKeyDown(Steins::Key::Up))
		{
			STEINS_INFO("GOOD");
		}*/
	}

	void OnEvent(Steins::Event& _event) override
	{
		//STEINS_TRACE("{0}", _event);
	}
};

class Sandbox : public Steins::Application
{
public:
	Sandbox()
		:Steins::Application()
	{
		AttachLayer(new ExampleLayer());
		//AttachOverlay(new Steins::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Steins::Application* Steins::CreateApplication()
{
	return new Sandbox();
}