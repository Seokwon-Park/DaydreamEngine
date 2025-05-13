#pragma once

#include "Steins/Core/Window.h"
#include "Steins/Core/LayerStack.h"
#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/ApplicationEvent.h"
#include "Steins/Render/GraphicsDevice.h"

#include "Steins/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Steins
{
	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int _index) const
		{
			STEINS_CORE_ASSERT(_index < count, "Out of index");
			return args[_index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Steins Application";
		std::string WorkingDirectory = "";
		UInt32 width = 1280;
		UInt32 height = 720;
		RendererAPIType rendererAPI = RendererAPIType::OpenGL;
	};

	class Application
	{
	public:
		// constrcuter destructer
		Application(ApplicationSpecification _specification);
		virtual ~Application();

		inline static Application& GetInstance() { return *instance; }
		inline static GraphicsDevice* GetGraphicsDevice() { return instance->graphicsDevice.get(); }
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
		Unique<SteinsWindow> testWindow;
		Unique<GraphicsDevice> graphicsDevice = nullptr;

		bool isRunning = false;
		static Application* instance;

		//Layers
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;

	};

	// To be defined in client
	Application* CreateApplication();
}

