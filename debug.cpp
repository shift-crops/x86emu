#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.hpp"

struct TypeSet {
	const char *name;
	FILE* fp;
	bool fatal;
};
TypeSet typeset[] = {
	{"ASSERT", stderr, true},
	{"ERROR", stderr, true},
	{"WARN", stderr, false},
	{"INFO", stdout, false},
	{NULL, stdout, false},
};

unsigned debug_level = 0;

void debug_print(const int type, const char *file, const char *function, int line, unsigned level, const char *fmt, ...){
	va_list ap;
	TypeSet ts = typeset[type];

	if(!ts.fatal && level > debug_level)
		return;

	if(ts.name){
		fprintf(ts.fp, level ? "[%s_%d] " : "[%s] ", ts.name, level);
		fprintf(ts.fp, "%s (%s:%d) ", function, file, line);
	}

	va_start(ap, fmt);
	vfprintf(ts.fp, fmt, ap);
	va_end(ap);

	if(ts.name)
		fprintf(ts.fp, "\n");

	if(ts.fatal)
		//exit(-1);
		throw -1;
}

void set_debuglv(const char *verbose){
	int i = 0;

	if(verbose)
		for(; verbose[i]=='v'; i++);

	debug_level = i+1;
}
