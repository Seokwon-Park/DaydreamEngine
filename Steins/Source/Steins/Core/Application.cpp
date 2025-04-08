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

	Application::Application()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		STEINS_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;
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
		RendererAPI::SetRendererAPI(API);

		mainWindow = SteinsWindow::Create();
		if (mainWindow == nullptr)
		{
			STEINS_CORE_ASSERT(false, "No Main Window")
				return false;
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init(mainWindow->GetGraphicsDevice());

		imGuiLayer = new ImGuiLayer();
		AttachOverlay(imGuiLayer);
		ImGuiLayer::Init();

		isRunning = true;

		float vertices[3 * 7] = {
					-0.5f, -0.5f, 0.0f, 1.0f, 0.0, 0.0f, 1.0f,
					 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
					 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		va = VertexArray::Create();

		vb = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vb->SetLayout(layout);
		va->AddVertexBuffer(vb);

		unsigned int indices[3] = { 0, 1, 2 };
		ib = IndexBuffer::Create(indices, 3);
		va->SetIndexBuffer(ib);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		shader = Shader::Create(vertexSrc,fragmentSrc);


		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
			RenderCommand::SetClearColor(Color::White);
			RenderCommand::Clear();

			m_Camera.SetRotation({ 0.5f, 0.5f, 0.0f });

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(shader, va);
			
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
