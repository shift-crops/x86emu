#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <unistd.h>

//#define DEBUG

enum {F_ERROR, F_WARN, F_INFO, F_MSG};
typedef struct {
	const char *name;
	FILE* fp;
	bool exit;
} TypeSet;

#define DEBUG_PRINT(type, fmt, ...)	debug_print(type, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef DEBUG
#define ON_DEBUG(type, fmt, ...)	DEBUG_PRINT(type, fmt, ##__VA_ARGS__)
#else
#define ON_DEBUG(type, fmt, ...)
#endif

#define ERROR(fmt, ...)			DEBUG_PRINT(F_ERROR, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)			ON_DEBUG(F_WARN, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)			ON_DEBUG(F_INFO, fmt, ##__VA_ARGS__)
#define DEBUG_MSG(fmt, ...)		ON_DEBUG(F_MSG, fmt, ##__VA_ARGS__)
#define MSG(fmt, ...)			fprintf(stdout, fmt, ##__VA_ARGS__)
 
void debug_print(const int type, const char *file, const char *function, int line, const char *fmt, ...);

#endif
