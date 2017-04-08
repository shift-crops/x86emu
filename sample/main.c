#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long

extern int _puts(char *);

int main(void){
	uint8_t *vram = (uint8_t*)0xa0000;

	_puts("start");
	for(int i=1; i<0x10; i++){
		for(int j=0; j<200*320; j++)
			vram[j] = i;
		__asm__("hlt");
	}

	for(int i=0; i<320*200; i++)
		vram[i] = i % 0x10;
/*
	for(int i=0; i<200; i++){
		uint8_t c8 = i % 0x10;
		uint32_t c32 = (c8<<24) + (c8<<16) + (c8<<8) + c8;
		for(int j=0; j<320/4; j++)
			((uint32_t*)vram)[i*80+j] = c32;
	}
*/
	while(1){
		_puts("puts in main function");
		//for(int i=0; i<1000; i++);
		__asm__("hlt");
	}
}
