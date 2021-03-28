#include "common.h"
int _puts(char *);
void set_graphicmode(void);

int main(void){
	uint8_t *vram = (uint8_t*)0xa0000;

	_puts("Hello!\n");

	_puts("Key or Mouse\n");
	__asm__("hlt");

	set_graphicmode();
	for(int i=0; i<0x10; i++){
		for(int j=0; j<320*200; j++)
			vram[j] = i;
		__asm__("hlt");
	}

	for(int i=0; i<320*200; i++)
		vram[i] = i % 0x10;
	__asm__("hlt");

	for(int i=0; i<200; i++){
		uint8_t c8 = i % 0x10;
		uint32_t c32 = (c8<<24) + (c8<<16) + (c8<<8) + c8;
		for(int j=0; j<320/4; j++)
			((uint32_t*)vram)[i*80+j] = c32;
	}

	return 0;
}
