#pragma once

#include "Steins/Window.h"
#include "Steins/Event/ApplicationEvent.h"


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

		void OnEvent(Event& _e);
		bool OnWindowClose(WindowCloseEvent& _e);

		Unique<SteinsWindow> mainWindow;
		bool isRunning = false;
		static Application* instance;
	};

	// To be defined in client
	Application* CreateApplication();
}

