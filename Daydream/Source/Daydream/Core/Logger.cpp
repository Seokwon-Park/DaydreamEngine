#include "DaydreamPCH.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Daydream
{
	std::shared_ptr<spdlog::logger> Logger::coreLogger;
	std::shared_ptr<spdlog::logger> Logger::clientLogger;

	void Logger::Init()
	{
		//https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		spdlog::set_pattern("%^[%T] %n : %v%$");
		coreLogger = spdlog::stdout_color_mt("Daydream ENGINE");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = spdlog::stdout_color_mt("APPLICATION");
		clientLogger->set_level(spdlog::level::trace);
	}

	void Logger::Shutdown()
	{
		spdlog::shutdown();
	}
}