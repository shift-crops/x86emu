#include "vga.h"

void attr_configure(void){
	cli();
	for(int i=0; i<0x10; i++){
		out_port(0x3c0, i);
		out_port(0x3c1, i);
	}
	sti();
}

void dac_configure(void){
	cli();
	out_port(0x3c8, 0);
	for(int i=0; i<0x100; i++){
		out_port(0x3c9, palette[i].red);
		out_port(0x3c9, palette[i].green);
		out_port(0x3c9, palette[i].blue);
	}
	sti();
}

void load_font(void *font){
}
