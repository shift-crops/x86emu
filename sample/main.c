#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long

extern int _puts(char *);

int main(void){
	uint8_t *vram = (uint8_t*)0xa0000;

	_puts("start");
	for(int i=0; i<320*200; i++)
		vram[i] = i % 0x10;
	while(1){
		_puts("puts in main function");
		//for(int i=0; i<1000; i++);
		__asm__("hlt");
	}
}
