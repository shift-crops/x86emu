#include "bios.h"

int bios_main(void){
	return 0;
}

void bios_init(void){
	init_ivt();
}

void init_ivt(void){
	set_ivt(0x10, (uint32_t)bsv_test, 0xf000);
	set_ivt(0x13, (uint32_t)bsv_disk, 0xf000);
	set_ivt(0x26, (uint32_t)bsv_irq_disk, 0xf000);
}

void set_ivt(int n, uint32_t offset, uint16_t cs){
	IVT* ivt = (IVT*)0;

	put_esmem(&(ivt[n].offset), offset);
	put_esmem(&(ivt[n].segment), cs);
}
