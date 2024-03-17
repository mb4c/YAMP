#pragma once

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

class Log
{
public:
	template<typename... Args>
	static void Trace(spdlog::format_string_t<Args...> fmt, Args&& ... args)
	{
		spdlog::trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Info(spdlog::format_string_t<Args...> fmt, Args&& ... args)
	{
		spdlog::info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Warn(spdlog::format_string_t<Args...> fmt, Args&& ... args)
	{
		spdlog::warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Error(spdlog::format_string_t<Args...> fmt, Args&& ... args)
	{
		spdlog::error(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Critical(spdlog::format_string_t<Args...> fmt, Args&& ... args)
	{
		spdlog::critical(fmt, std::forward<Args>(args)...);
	}
};