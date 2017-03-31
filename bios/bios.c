#include "bios.h"

int bios_main(void){
	init_ivt();

	return 0;
}

void init_ivt(void){
	set_ivt(0x10, (uint32_t)bsv_test, 0xf000);
}

void set_ivt(int n, uint32_t offset, uint16_t cs){
	IVT* ivt = (IVT*)0;

	put_esmem(&(ivt[n].offset), offset);
	put_esmem(&(ivt[n].segment), cs);
}
