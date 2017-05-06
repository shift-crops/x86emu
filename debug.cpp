#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.hpp"

TypeSet typeset[] = {
	{"ERROR", STDERR_FILENO, true},
	{"WARN", STDERR_FILENO, false},
	{"INFO", STDOUT_FILENO, false},
	{NULL, STDOUT_FILENO, false},
};

void debug_print(const int type, const char *file, const char *function, int line, const char *fmt, ...){
	va_list ap;
	TypeSet ts = typeset[type];

	if(ts.name)
		dprintf(ts.fd, "[%s] %s (%s:%d) ", ts.name, function, file, line);

	va_start(ap, fmt);
	vdprintf(ts.fd, fmt, ap);
	va_end(ap);

	if(ts.name)
		dprintf(ts.fd, "\n");

	if(ts.exit)
		//exit(-1);
		throw -1;
}
