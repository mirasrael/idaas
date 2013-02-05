#pragma once

#pragma warning(disable: 4996)
#include <idp.hpp>
#pragma warning(default: 4996)

extern "C" bool idaapi isLoggingEnabled();
extern "C" void idaapi setLoggingEnabled(bool enabled);
extern "C" int idaapi logmsg(const char *format, ...);