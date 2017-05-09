#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <unistd.h>

#define DEBUG

enum {F_ASSERT, F_ERROR, F_WARN, F_INFO, F_MSG};

#define DEBUG_PRINT(type, lv, fmt, ...)	debug_print(type, __FILE__, __FUNCTION__, __LINE__, lv, fmt, ##__VA_ARGS__)

#ifdef DEBUG
#define ON_DEBUG(type, lv, fmt, ...)	DEBUG_PRINT(type, lv, fmt, ##__VA_ARGS__)
#define DEBUG_OPT			"v::"
#else
#define ON_DEBUG(type, lv, fmt, ...)
#define DEBUG_OPT
#endif

#define ASSERT(cond)			if(!(cond)) DEBUG_PRINT(F_ASSERT, 0, #cond)
#define ERROR(fmt, ...)			DEBUG_PRINT(F_ERROR, 0, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)			ON_DEBUG(F_WARN, 0, fmt, ##__VA_ARGS__)
#define INFO(lv, fmt, ...)		ON_DEBUG(F_INFO, lv, fmt, ##__VA_ARGS__)
#define DEBUG_MSG(lv, fmt, ...)		ON_DEBUG(F_MSG, lv, fmt, ##__VA_ARGS__)
#define MSG(fmt, ...)			fprintf(stdout, fmt, ##__VA_ARGS__)
 
void debug_print(const int type, const char *file, const char *function, int line, unsigned level, const char *fmt, ...);
void set_debuglv(const char *verbose);

#endif
