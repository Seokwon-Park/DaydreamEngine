#pragma once

#include "Steins/Window.h"
#include "Steins/LayerStack.h"
#include "Steins/Event/KeyEvent.h"
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

		static Application& GetInstance() { return *instance; }
		inline SteinsWindow& GetMainWindow() { return *mainWindow; }
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
		LayerStack Layers;
	};

	// To be defined in client
	Application* CreateApplication();
}

