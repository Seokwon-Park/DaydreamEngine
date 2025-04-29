#include "SteinsPCH.h"

#include "Application.h"

#include "Input.h"
#include "KeyCodes.h"
#include "Steins/ImGui/ImGuiLayer.h"
#include "Steins/Render/Renderer.h"

#include "glad/glad.h"



namespace Steins
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application(ApplicationSpecification _specification)
	{
		STEINS_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		WindowProps prop;
		prop.width = 1280;
		prop.height = 720;
		prop.title = _specification.Name;
		prop.rendererAPI = _specification.rendererAPI;

		mainWindow = SteinsWindow::Create(prop);
		if (mainWindow == nullptr)
		{
			STEINS_CORE_ASSERT(false, "No Main Window")
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mainWindow->SetVSync(true);

		graphicsDevice = GraphicsDevice::Create(_specification.rendererAPI);
		STEINS_CORE_ASSERT(graphicsDevice, "Failed to create graphics device!");
		graphicsDevice->Init();

		//TODO : 더 좋은 방법?
		SwapChainSpecification desc;
		desc.width = mainWindow->GetWidth();
		desc.height = mainWindow->GetHeight();
		desc.bufferCount = 2;
		desc.format = RenderFormat::R8G8B8A8_UNORM;
		desc.isFullscreen = false;
		desc.isVSync = mainWindow->IsVSync();

		Shared<SwapChain> swapchain = graphicsDevice->CreateSwapChain(&desc, mainWindow.get());
		mainWindow->SetSwapchain(swapchain);

		//testWindow = SteinsWindow::Create();
		//testWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init(graphicsDevice.get());

		imGuiLayer = new ImGuiLayer();
		AttachOverlay(imGuiLayer);


		//glGenVertexArrays(1, &m_VertexArray);
		//glBindVertexArray(m_VertexArray);

		//glGenBuffers(1, &m_VertexBuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);


		//float vertices[3 * 7] = {
		//	-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,1.0f,
		//	 0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,
		//	 0.0f,  0.5f, 0.0f,1.0f,0.0f,0.0f,1.0f
		//};

		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0));

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

		//glGenBuffers(1, &m_IndexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//unsigned int indices[3] = { 0, 1, 2 };
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{
		mainWindow->SetSwapchain(nullptr);
		layerStack.Release();
		mainWindow = nullptr;
	}

	void Application::AttachLayer(Layer* _layer)
	{
		layerStack.PushLayer(_layer);
	}

	void Application::AttachOverlay(Layer* _overlay)
	{
		layerStack.PushOverlay(_overlay);
	}

	void Application::ReadConfig(std::string_view _fileName)
	{
		std::ifstream file(_fileName.data());
		STEINS_CORE_ASSERT(!file.is_open(), "Cannot Open Configuration File!")

			std::string line;

		/*while (std::getline(file, line)) {
			if (line.find("API=") != std::string::npos) {
				api = line.substr(line.find("=") + 1);
				break;
			}
		}*/
	}

	bool Application::Init()
	{
		isRunning = true;

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			//glBindVertexArray(m_VertexArray);
			//glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr, 0);


			imGuiLayer->BeginImGui();
			{
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
			}
			imGuiLayer->EndImGui();

			//auto [x, y] = Input::GetMousePosition();
			////STEINS_CORE_TRACE("{0}, {1}", x, y);

			//if (Input::GetMouseButtonPress(Mouse::ButtonLeft))
			//{
			//	STEINS_CORE_TRACE("MOUSE BUTTON DOWN TEST");
			//}

			//if (Input::GetKeyDown(Key::A))
			//{
			//	STEINS_CORE_TRACE("KEY DOWN TEST");
			//}

			//if (Input::GetKeyPress(Key::B))
			//{
			//	STEINS_CORE_TRACE("KEY PRESS TEST");
			//}

			//if (Input::GetKeyUp(Key::C))
			//{
			//	STEINS_CORE_TRACE("KEY UP TEST");
			//}

			mainWindow->OnUpdateKeyState();
			mainWindow->OnUpdate();
			//testWindow->OnUpdate();
		}
		return true;
	}
	bool Application::Exit()
	{
		return true;
	}

	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		//_event의 타입이 WindowCloseEvent에 해당하면 OnWindowClose를 실행시킨다.
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (std::vector<Layer*>::iterator itr = layerStack.end(); itr != layerStack.begin();)
		{
			if (_event.handled) // if Event is already handled then break;
				break;
			//check event for layer
			(*--itr)->OnEvent(_event);
		}

		//STEINS_CORE_TRACE("{0}", _event.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& _event)
	{
		isRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& _e)
	{
		return false;
	}
}
