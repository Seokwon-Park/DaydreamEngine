#pragma once

#include "Steins/Window.h"
#include "Steins/Core/LayerStack.h"
#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/ApplicationEvent.h"

#include "Steins/ImGui/ImGuiLayer.h"

#include "Steins/Enum/RendererAPI.h"

int main(int argc, char** argv);

namespace Steins
{
	class Application
	{
	public:
		// constrcuter destructer
		Application();
		virtual ~Application();

		inline static Application& GetInstance() { return *instance; }
		inline static RendererAPI GetRendererAPI() { return instance->rendererAPI; }
		inline SteinsWindow& GetMainWindow() { return *mainWindow; }

		void AttachLayer(Layer* _layer);
		void AttachOverlay(Layer* _overlay);

		void ReadConfig(std::string_view _fileName);
	protected:

	private:
		friend int ::main(int argc, char** argv);

		bool Init();
		bool Run();
		bool Exit();

		void OnEvent(Event& _e);
		bool OnWindowClose(WindowCloseEvent& _e);
		bool OnWindowResize(WindowResizeEvent& _e);

		Unique<SteinsWindow> mainWindow;
		bool isRunning = false;
		static Application* instance;

		//Layers
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;

		//RenderOptions
		RendererAPI rendererAPI;
		
	};

	// To be defined in client
	Application* CreateApplication();
}

