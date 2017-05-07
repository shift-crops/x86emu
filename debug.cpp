#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.hpp"

TypeSet typeset[] = {
	{"ERROR", stderr, true},
	{"WARN", stderr, false},
	{"INFO", stdout, false},
	{NULL, stdout, false},
};

void debug_print(const int type, const char *file, const char *function, int line, const char *fmt, ...){
	va_list ap;
	TypeSet ts = typeset[type];

	if(ts.name)
		fprintf(ts.fp, "[%s] %s (%s:%d) ", ts.name, function, file, line);

	va_start(ap, fmt);
	vfprintf(ts.fp, fmt, ap);
	va_end(ap);

	if(ts.name)
		fprintf(ts.fp, "\n");

	if(ts.exit)
		//exit(-1);
		throw -1;
}
