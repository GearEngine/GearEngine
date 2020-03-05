#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace Gear {

	class GEAR_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

//Core log macro
#define GR_CORE_TRACE(...)		::Gear::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GR_CORE_INFO(...)		::Gear::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GR_CORE_WARN(...)		::Gear::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GR_CORE_ERROR(...)		::Gear::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GR_CORE_FATAL(...)		::Gear::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macro
#define GR_TRACE(...)			::Gear::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GR_INFO(...)			::Gear::Log::GetClientLogger()->info(__VA_ARGS__)
#define GR_WARN(...)			::Gear::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GR_ERROR(...)			::Gear::Log::GetClientLogger()->error(__VA_ARGS__)
#define GR_FATAL(...)			::Gear::Log::GetClientLogger()->fatal(__VA_ARGS__)
