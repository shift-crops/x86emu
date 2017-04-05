#ifndef _BIOS_H
#define _BIOS_H

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long

typedef struct {
	uint16_t offset;
	uint16_t segment;
} IVT;

extern void bsv_test(void);
extern void bsv_disk(void);
extern void bsv_irq_disk(void);

void put_esmem(void* vaddr, uint16_t v);
void init_ivt(void);
void set_ivt(int n, uint32_t offset, uint16_t cs);

#endif
