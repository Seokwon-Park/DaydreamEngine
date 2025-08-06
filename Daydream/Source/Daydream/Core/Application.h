#pragma once

#include "Base/Time/TimeStep.h"

#include "Daydream/Core/Window.h"
#include "Daydream/Core/WindowManager.h"
#include "Daydream/Core/LayerStack.h"
#include "Daydream/Event/KeyEvent.h"
#include "Daydream/Event/ApplicationEvent.h"

#include "Daydream/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Daydream
{
	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int _index) const
		{
			DAYDREAM_CORE_ASSERT(_index < count, "Out of index");
			return args[_index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Daydream Application";
		std::string WorkingDirectory = "";
		UInt32 width = 1280;
		UInt32 height = 720;
		RendererAPIType rendererAPI = RendererAPIType::OpenGL;
	};

	class DAYDREAM_API Application
	{
	public:
		Application(ApplicationSpecification _specification);
		virtual ~Application();

		inline static Application& GetInstance() { return *instance; }
		inline DaydreamWindow& GetMainWindow() { return *mainWindow; }

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
		bool OnWindowFocused(WindowFocusEvent& _e);

		Unique<DaydreamWindow> mainWindow;
		//Unique<DaydreamWindow> testWindow;

		bool isRunning = false;
		bool isMinimized = false;
		static Application* instance;

		//Layers
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;

		TimeStep timeStep;
	};

	// To be defined in client
	Application* CreateApplication();
}

