#pragma once

#define FMT_UNICODE 0

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Steins
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
#define STEINS_CORE_TRACE(...)		::Steins::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define STEINS_CORE_INFO(...)		::Steins::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define STEINS_CORE_WARN(...)		::Steins::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define STEINS_CORE_ERROR(...)		::Steins::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define STEINS_CORE_FATAL(...)		::Steins::Logger::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define STEINS_TRACE(...)			::Steins::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define STEINS_INFO(...)			::Steins::Logger::GetClientLogger()->info(__VA_ARGS__)
#define STEINS_WARN(...)			::Steins::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define STEINS_ERROR(...)			::Steins::Logger::GetClientLogger()->error(__VA_ARGS__)
#define STEINS_FATAL(...)			::Steins::Logger::GetClientLogger()->fatal(__VA_ARGS__)


