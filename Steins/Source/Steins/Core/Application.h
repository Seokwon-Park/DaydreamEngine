#pragma once

int main(int argc, char** argv);

namespace Steins
{
	class Application
	{
	public:
		// constrcuter destructer
		Application();
		virtual ~Application();

	protected:

	private:
		friend int ::main(int argc, char** argv);

		bool Init();
		bool Run();
		bool Exit();

		bool isRunning = false;
		static Application* instance;
	};

	// To be defined in client
	Application* CreateApplication();
}

