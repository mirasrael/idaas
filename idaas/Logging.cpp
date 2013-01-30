#include "Logging.h"

bool g_loggingEnabled = true;

bool idaapi isLoggingEnabled() {
	return g_loggingEnabled;
}

void idaapi setLoggingEnabled(bool enabled) {
	g_loggingEnabled = enabled;
}

int idaapi logmsg(const char *format, ...) {
	if (!isLoggingEnabled())
		return -1;

	va_list va;
	va_start(va, format);
	int nbytes = vmsg(format, va);
	va_end(va);
	return nbytes;
}
