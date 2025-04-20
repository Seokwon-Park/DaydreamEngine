#include "Base/MathTypes.h"

namespace Steins
{
	extern Application* CreateApplication();
}

Int32 main(int argc, char** argv)
{
	Steins::Logger::Init();

	Steins::Application* application = Steins::CreateApplication();

	if (application->Init() == false)
	{
		return 1;
	}

	application->Run();

	if (!application->Exit())
	{
		return 1;
	}

	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}

	return 0;
}
