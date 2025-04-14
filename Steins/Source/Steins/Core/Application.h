#pragma once

#include "Steins/Core/Window.h"
#include "Steins/Core/LayerStack.h"
#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/ApplicationEvent.h"

#include "Steins/ImGui/ImGuiLayer.h"

#include "Steins/Render/RendererAPI.h"

#include "Steins/Render/Shader.h"
#include "Steins/Render/Buffer.h"
#include "Steins/Render/VertexArray.h"

#include "Steins/Render/OrthographicCamera.h"

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
		inline static GraphicsDevice* GetGraphicsDevice() { return instance->graphicsDevice; }
		inline SteinsWindow& GetMainWindow() { return *mainWindow; }

		void AttachLayer(Layer* _layer);
		void AttachOverlay(Layer* _overlay);

		void ReadConfig(std::string_view _fileName);

		template <typename DeviceType>
		DeviceType* GetNativeDevice() const { return DynamicCast<DeviceType>(graphicsDevice); }
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
		Unique<SteinsWindow> TestWindow;
		GraphicsDevice* graphicsDevice = nullptr;

		bool isRunning = false;
		static Application* instance;

		//Layers
		ImGuiLayer* imGuiLayer;
		LayerStack layerStack;

		RendererAPIType API = RendererAPIType::OpenGL;	
	};

	// To be defined in client
	Application* CreateApplication();
}

