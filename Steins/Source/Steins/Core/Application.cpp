#include "SteinsPCH.h"

#include "Application.h"
#include "Logger.h"

#include "Steins/Event/ApplicationEvent.h"

namespace Steins
{
	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	bool Application::Init()
	{
		Logger::Init();

		isRunning = true;

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
		}
		return true;
	}
	bool Application::Exit()
	{
		return true;
	}
}
