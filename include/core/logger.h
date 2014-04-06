#pragma once

#include <stdarg.h>

namespace baldr
{
	namespace logger
	{
		//assign to that the wanted logger.
		//Engine init put it by default to consoleLog
		extern void (*logFunc)(const char*, va_list);

		void log(const char* str, ...);
	}

	namespace loggerimpl
	{
		void consoleLog(const char*, va_list);
	}
}