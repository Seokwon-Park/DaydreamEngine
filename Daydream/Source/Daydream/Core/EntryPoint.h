#include "Base/MathTypes.h"

namespace Daydream
{
	extern Application* CreateApplication();
}

Int32 main(int argc, char** argv)
{
	Daydream::Logger::Init();

	Daydream::Application* application = Daydream::CreateApplication();

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
