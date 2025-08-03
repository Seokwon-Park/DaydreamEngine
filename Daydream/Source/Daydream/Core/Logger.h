#pragma once

#define FMT_UNICODE 0

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Daydream
{
	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
	
}

// Core log macros
#define DAYDREAM_CORE_TRACE(...)		::Daydream::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define DAYDREAM_CORE_INFO(...)		::Daydream::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define DAYDREAM_CORE_WARN(...)		::Daydream::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define DAYDREAM_CORE_ERROR(...)		::Daydream::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define DAYDREAM_CORE_FATAL(...)		::Daydream::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DAYDREAM_TRACE(...)			::Daydream::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define DAYDREAM_INFO(...)			::Daydream::Logger::GetClientLogger()->info(__VA_ARGS__)
#define DAYDREAM_WARN(...)			::Daydream::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define DAYDREAM_ERROR(...)			::Daydream::Logger::GetClientLogger()->error(__VA_ARGS__)
#define DAYDREAM_FATAL(...)			::Daydream::Logger::GetClientLogger()->critical(__VA_ARGS__)


