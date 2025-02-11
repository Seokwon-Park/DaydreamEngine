#pragma once

int main(int argc, char** argv);

namespace Steins
{
	class Application
	{
	public:
		// constrcuter destructer
		Application(const std::string_view _name);
		virtual ~Application();

		// delete Function
		Application(const Application& _other) = delete;
		Application(Application&& _other) noexcept = delete;
		Application& operator=(const Application& _other) = delete;
		Application& operator=(Application&& _other) noexcept = delete;


	protected:

	private:
		bool Init();
		bool Run();
		bool Exit();

		friend int ::main(int argc, char** argv);
		static Application* instance;
	};

	// To be defined in client
	Application* CreateApplication();
}

