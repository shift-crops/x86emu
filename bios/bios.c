#include "bios.h"

char *msg = "now booting from floppy disk...";

int bios_main(void){
	print(msg);
	return 0;
}

void bios_init(void){
	init_ivt();
}

void init_ivt(void){
	set_ivt(0x10, (uint32_t)bsv_video, 0xf000);
	set_ivt(0x13, (uint32_t)bsv_disk, 0xf000);
	set_ivt(0x16, (uint32_t)bsv_keyboard, 0xf000);
	set_ivt(0x26, (uint32_t)bsv_irq_disk, 0xf000);
}

void set_ivt(int n, uint32_t offset, uint16_t cs){
	IVT* ivt = (IVT*)0;

	write_esw(&(ivt[n].offset), offset);
	write_esw(&(ivt[n].segment), cs);
}
