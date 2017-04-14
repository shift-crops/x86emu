#ifndef _BIOS_H
#define _BIOS_H

#include "common.h"

typedef struct {
	uint16_t offset;
	uint16_t segment;
} IVT;

void init_ivt(void);
void set_ivt(int n, uint32_t offset, uint16_t cs);

#endif
