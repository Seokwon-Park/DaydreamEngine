#include "SteinsPCH.h"

#include "Application.h"

#include "Input.h"
#include "KeyCodes.h"
#include "Steins/ImGui/ImGuiLayer.h"

#include "glad/glad.h"


namespace Steins
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application()
	{
		STEINS_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		mainWindow = SteinsWindow::Create();

		imGuiLayer = new ImGuiLayer();
		AttachOverlay(imGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{

	}

	void Application::AttachLayer(Layer* _layer)
	{
		layerStack.PushLayer(_layer);
		_layer->OnAttach();
	}

	void Application::AttachOverlay(Layer* _overlay)
	{
		layerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
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
		if (mainWindow == nullptr)
		{
			STEINS_CORE_ASSERT(false, "No Main Window")
			return false;
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		ImGuiLayer::Init(mainWindow.get());
		isRunning = true;

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{

			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);


			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			imGuiLayer->BeginImGui();
			{
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
			}
			imGuiLayer->EndImGui();

			auto [x, y] = Input::GetMousePosition();
			//STEINS_CORE_TRACE("{0}, {1}", x, y);

			if (Input::GetMouseButtonPress(Mouse::ButtonLeft))
			{
				STEINS_CORE_TRACE("MOUSE BUTTON DOWN TEST");
			}

			if (Input::GetKeyDown(Key::A))
			{
				STEINS_CORE_TRACE("KEY DOWN TEST");
			}

			if (Input::GetKeyPress(Key::B))
			{
				STEINS_CORE_TRACE("KEY PRESS TEST");
			}

			if (Input::GetKeyUp(Key::C))
			{
				STEINS_CORE_TRACE("KEY UP TEST");
			}

			mainWindow->OnUpdateKeyState();
			mainWindow->OnUpdate();
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
