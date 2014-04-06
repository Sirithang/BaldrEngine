#include "core/logger.h"

#include <cstdio>

void (*baldr::logger::logFunc)(const char*, va_list) = NULL;

void baldr::logger::log(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	logFunc(str, ap);
	va_end (ap);
}

//============================================================


void baldr::loggerimpl::consoleLog(const char* str, va_list arg)
{
	vprintf(str, arg);
	printf("\n");
}