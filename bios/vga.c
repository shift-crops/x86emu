#include "common.h"
#include "font8x8.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} rgb_t;

const rgb_t palette[0x10] = {
//       R,    G,    B
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x2a},
	{0x00, 0x2a, 0x00},
	{0x00, 0x2a, 0x2a},
	{0x2a, 0x00, 0x00},
	{0x2a, 0x00, 0x2a},
	{0x2a, 0x15, 0x00},
	{0x2a, 0x2a, 0x2a},
	{0x15, 0x15, 0x15},
	{0x15, 0x15, 0x3f},
	{0x15, 0x3f, 0x15},
	{0x15, 0x3f, 0x3f},
	{0x3f, 0x15, 0x15},
	{0x3f, 0x15, 0x3f},
	{0x3f, 0x3f, 0x15},
	{0x3f, 0x3f, 0x3f}
};

void attr_configure(void){
	for(int i=0; i<0x10; i++){
		out_port(0x3c0, i);
		out_port(0x3c1, i);
	}
}

void seq_configure(void){
	out_port(0x3c4, 2);
	out_port(0x3c5, 0x3);

	out_port(0x3c4, 3);
	out_port(0x3c5, 0x0);

	out_port(0x3c4, 4);
	out_port(0x3c5, 0x2);
}

void dac_configure(void){
	out_port(0x3c8, 0);
	for(int i=0; i<0x10; i++){
		out_port(0x3c9, palette[i].red);
		out_port(0x3c9, palette[i].green);
		out_port(0x3c9, palette[i].blue);
	}
}

void gc_configure(void){
	out_port(0x3ce, 5);
	out_port(0x3cf, 0x10);

	out_port(0x3ce, 6);
	out_port(0x3cf, 0xe);
}

void load_font(void){
	_cli();
	out_port(0x3c4, 2);
	out_port(0x3c5, 0x4);
	out_port(0x3c4, 4);
	out_port(0x3c5, 0x6);

	out_port(0x3ce, 5);
	out_port(0x3cf, 0x0);
	out_port(0x3ce, 6);
	out_port(0x3cf, 0x0);
	_sti();

	__asm__("push es\n"
		"mov ax, 0xa000\n"
		"mov es, ax");

	for(int i=0; i<0x80; i++){
		uint8_t *p = font8x8_basic[i];
		write_esd((uint32_t*)(i*0x10), *(uint32_t*)&p[0]);
		write_esd((uint32_t*)(i*0x10+4), *(uint32_t*)&p[4]);
	}
	__asm__("pop es");
}

extern uint16_t cursor_x, cursor_y;
uint32_t print(const uint8_t *s){
	uint32_t i;

	__asm__("push es\n"
		"mov ax, 0xb800\n"
		"mov es, ax");

	for(i=0; s[i]; i++){
		write_esw((uint16_t*)((cursor_y*0x28 + cursor_x)*2), 0x0700 + s[i]);
		cursor_x++;
		if(cursor_x >= 0x28 || !(s[i]^0x0a)){
			cursor_x = 0;
			cursor_y++;
		}

		if(cursor_y >= 0x19){
			uint32_t j;
			for(j=0; j<0x18*0x28; j++)
				copy_esw((uint16_t*)(j*2), (uint16_t*)((0x28+j)*2));
			for(; j<0x19*0x28; j++)
				write_esw((uint16_t*)(j*2), 0x0700);
			cursor_x = 0;
			cursor_y--;
		}
	}

	__asm__("pop es");

	return i;
}

