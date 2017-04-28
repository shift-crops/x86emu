#include "common.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} rgb_t;

const rgb_t palette[0x100] = {
//	 R,    G,    B
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
	{0x3f, 0x3f, 0x3f},
	{0x00, 0x00, 0x00},
	{0x05, 0x05, 0x05},
	{0x08, 0x08, 0x08},
	{0x0b, 0x0b, 0x0b},
	{0x0e, 0x0e, 0x0e},
	{0x11, 0x11, 0x11},
	{0x14, 0x14, 0x14},
	{0x18, 0x18, 0x18},
	{0x1c, 0x1c, 0x1c},
	{0x20, 0x20, 0x20},
	{0x24, 0x24, 0x24},
	{0x28, 0x28, 0x28},
	{0x2d, 0x2d, 0x2d},
	{0x32, 0x32, 0x32},
	{0x38, 0x38, 0x38},
	{0x3f, 0x3f, 0x3f},
	{0x00, 0x00, 0x3f},
	{0x10, 0x00, 0x3f},
	{0x1f, 0x00, 0x3f},
	{0x2f, 0x00, 0x3f},
	{0x3f, 0x00, 0x3f},
	{0x3f, 0x00, 0x2f},
	{0x3f, 0x00, 0x1f},
	{0x3f, 0x00, 0x10},
	{0x3f, 0x00, 0x00},
	{0x3f, 0x10, 0x00},
	{0x3f, 0x1f, 0x00},
	{0x3f, 0x2f, 0x00},
	{0x3f, 0x3f, 0x00},
	{0x2f, 0x3f, 0x00},
	{0x1f, 0x3f, 0x00},
	{0x10, 0x3f, 0x00},
	{0x00, 0x3f, 0x00},
	{0x00, 0x3f, 0x10},
	{0x00, 0x3f, 0x1f},
	{0x00, 0x3f, 0x2f},
	{0x00, 0x3f, 0x3f},
	{0x00, 0x2f, 0x3f},
	{0x00, 0x1f, 0x3f},
	{0x00, 0x10, 0x3f},
	{0x1f, 0x1f, 0x3f},
	{0x27, 0x1f, 0x3f},
	{0x2f, 0x1f, 0x3f},
	{0x37, 0x1f, 0x3f},
	{0x3f, 0x1f, 0x3f},
	{0x3f, 0x1f, 0x37},
	{0x3f, 0x1f, 0x2f},
	{0x3f, 0x1f, 0x27},
	{0x3f, 0x1f, 0x1f},
	{0x3f, 0x27, 0x1f},
	{0x3f, 0x2f, 0x1f},
	{0x3f, 0x37, 0x1f},
	{0x3f, 0x3f, 0x1f},
	{0x37, 0x3f, 0x1f},
	{0x2f, 0x3f, 0x1f},
	{0x27, 0x3f, 0x1f},
	{0x1f, 0x3f, 0x1f},
	{0x1f, 0x3f, 0x27},
	{0x1f, 0x3f, 0x2f},
	{0x1f, 0x3f, 0x37},
	{0x1f, 0x3f, 0x3f},
	{0x1f, 0x37, 0x3f},
	{0x1f, 0x2f, 0x3f},
	{0x1f, 0x27, 0x3f},
	{0x2d, 0x2d, 0x3f},
	{0x31, 0x2d, 0x3f},
	{0x36, 0x2d, 0x3f},
	{0x3a, 0x2d, 0x3f},
	{0x3f, 0x2d, 0x3f},
	{0x3f, 0x2d, 0x3a},
	{0x3f, 0x2d, 0x36},
	{0x3f, 0x2d, 0x31},
	{0x3f, 0x2d, 0x2d},
	{0x3f, 0x31, 0x2d},
	{0x3f, 0x36, 0x2d},
	{0x3f, 0x3a, 0x2d},
	{0x3f, 0x3f, 0x2d},
	{0x3a, 0x3f, 0x2d},
	{0x36, 0x3f, 0x2d},
	{0x31, 0x3f, 0x2d},
	{0x2d, 0x3f, 0x2d},
	{0x2d, 0x3f, 0x31},
	{0x2d, 0x3f, 0x36},
	{0x2d, 0x3f, 0x3a},
	{0x2d, 0x3f, 0x3f},
	{0x2d, 0x3a, 0x3f},
	{0x2d, 0x36, 0x3f},
	{0x2d, 0x31, 0x3f},
	{0x00, 0x00, 0x1c},
	{0x07, 0x00, 0x1c},
	{0x0e, 0x00, 0x1c},
	{0x15, 0x00, 0x1c},
	{0x1c, 0x00, 0x1c},
	{0x1c, 0x00, 0x15},
	{0x1c, 0x00, 0x0e},
	{0x1c, 0x00, 0x07},
	{0x1c, 0x00, 0x00},
	{0x1c, 0x07, 0x00},
	{0x1c, 0x0e, 0x00},
	{0x1c, 0x15, 0x00},
	{0x1c, 0x1c, 0x00},
	{0x15, 0x1c, 0x00},
	{0x0e, 0x1c, 0x00},
	{0x07, 0x1c, 0x00},
	{0x00, 0x1c, 0x00},
	{0x00, 0x1c, 0x07},
	{0x00, 0x1c, 0x0e},
	{0x00, 0x1c, 0x15},
	{0x00, 0x1c, 0x1c},
	{0x00, 0x15, 0x1c},
	{0x00, 0x0e, 0x1c},
	{0x00, 0x07, 0x1c},
	{0x0e, 0x0e, 0x1c},
	{0x11, 0x0e, 0x1c},
	{0x15, 0x0e, 0x1c},
	{0x18, 0x0e, 0x1c},
	{0x1c, 0x0e, 0x1c},
	{0x1c, 0x0e, 0x18},
	{0x1c, 0x0e, 0x15},
	{0x1c, 0x0e, 0x11},
	{0x1c, 0x0e, 0x0e},
	{0x1c, 0x11, 0x0e},
	{0x1c, 0x15, 0x0e},
	{0x1c, 0x18, 0x0e},
	{0x1c, 0x1c, 0x0e},
	{0x18, 0x1c, 0x0e},
	{0x15, 0x1c, 0x0e},
	{0x11, 0x1c, 0x0e},
	{0x0e, 0x1c, 0x0e},
	{0x0e, 0x1c, 0x11},
	{0x0e, 0x1c, 0x15},
	{0x0e, 0x1c, 0x18},
	{0x0e, 0x1c, 0x1c},
	{0x0e, 0x18, 0x1c},
	{0x0e, 0x15, 0x1c},
	{0x0e, 0x11, 0x1c},
	{0x14, 0x14, 0x1c},
	{0x16, 0x14, 0x1c},
	{0x18, 0x14, 0x1c},
	{0x1a, 0x14, 0x1c},
	{0x1c, 0x14, 0x1c},
	{0x1c, 0x14, 0x1a},
	{0x1c, 0x14, 0x18},
	{0x1c, 0x14, 0x16},
	{0x1c, 0x14, 0x14},
	{0x1c, 0x16, 0x14},
	{0x1c, 0x18, 0x14},
	{0x1c, 0x1a, 0x14},
	{0x1c, 0x1c, 0x14},
	{0x1a, 0x1c, 0x14},
	{0x18, 0x1c, 0x14},
	{0x16, 0x1c, 0x14},
	{0x14, 0x1c, 0x14},
	{0x14, 0x1c, 0x16},
	{0x14, 0x1c, 0x18},
	{0x14, 0x1c, 0x1a},
	{0x14, 0x1c, 0x1c},
	{0x14, 0x1a, 0x1c},
	{0x14, 0x18, 0x1c},
	{0x14, 0x16, 0x1c},
	{0x00, 0x00, 0x10},
	{0x04, 0x00, 0x10},
	{0x08, 0x00, 0x10},
	{0x0c, 0x00, 0x10},
	{0x10, 0x00, 0x10},
	{0x10, 0x00, 0x0c},
	{0x10, 0x00, 0x08},
	{0x10, 0x00, 0x04},
	{0x10, 0x00, 0x00},
	{0x10, 0x04, 0x00},
	{0x10, 0x08, 0x00},
	{0x10, 0x0c, 0x00},
	{0x10, 0x10, 0x00},
	{0x0c, 0x10, 0x00},
	{0x08, 0x10, 0x00},
	{0x04, 0x10, 0x00},
	{0x00, 0x10, 0x00},
	{0x00, 0x10, 0x04},
	{0x00, 0x10, 0x08},
	{0x00, 0x10, 0x0c},
	{0x00, 0x10, 0x10},
	{0x00, 0x0c, 0x10},
	{0x00, 0x08, 0x10},
	{0x00, 0x04, 0x10},
	{0x08, 0x08, 0x10},
	{0x0a, 0x08, 0x10},
	{0x0c, 0x08, 0x10},
	{0x0e, 0x08, 0x10},
	{0x10, 0x08, 0x10},
	{0x10, 0x08, 0x0e},
	{0x10, 0x08, 0x0c},
	{0x10, 0x08, 0x0a},
	{0x10, 0x08, 0x08},
	{0x10, 0x0a, 0x08},
	{0x10, 0x0c, 0x08},
	{0x10, 0x0e, 0x08},
	{0x10, 0x10, 0x08},
	{0x0e, 0x10, 0x08},
	{0x0c, 0x10, 0x08},
	{0x0a, 0x10, 0x08},
	{0x08, 0x10, 0x08},
	{0x08, 0x10, 0x0a},
	{0x08, 0x10, 0x0c},
	{0x08, 0x10, 0x0e},
	{0x08, 0x10, 0x10},
	{0x08, 0x0e, 0x10},
	{0x08, 0x0c, 0x10},
	{0x08, 0x0a, 0x10},
	{0x0b, 0x0b, 0x10},
	{0x0c, 0x0b, 0x10},
	{0x0d, 0x0b, 0x10},
	{0x0f, 0x0b, 0x10},
	{0x10, 0x0b, 0x10},
	{0x10, 0x0b, 0x0f},
	{0x10, 0x0b, 0x0d},
	{0x10, 0x0b, 0x0c},
	{0x10, 0x0b, 0x0b},
	{0x10, 0x0c, 0x0b},
	{0x10, 0x0d, 0x0b},
	{0x10, 0x0f, 0x0b},
	{0x10, 0x10, 0x0b},
	{0x0f, 0x10, 0x0b},
	{0x0d, 0x10, 0x0b},
	{0x0c, 0x10, 0x0b},
	{0x0b, 0x10, 0x0b},
	{0x0b, 0x10, 0x0c},
	{0x0b, 0x10, 0x0d},
	{0x0b, 0x10, 0x0f},
	{0x0b, 0x10, 0x10},
	{0x0b, 0x0f, 0x10},
	{0x0b, 0x0d, 0x10},
	{0x0b, 0x0c, 0x10},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00}
};
uint16_t cursor_x=0, cursor_y=0;
bool graphic = false;

void cli(void);
void sti(void);
void out_port(uint16_t port, uint8_t v);

void gc_configure(void){
	cli();
	out_port(0x3ce, 0x6);
	out_port(0x3cf, 0x1);
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

void init_vga(void){
	dac_configure();
	//gc_configure();
}

void scroll_page(uint8_t n){
	uint16_t i;
	uint16_t *vram = (uint16_t*)0xb8000;

	for(i=0; i<(0x19-n)*0x28; i++)
		vram[i] = vram[n*0x28+i];
	for(; i<0x19*0x28; i++)
		vram[i] = 0x0700;

	cursor_x = 0;
	cursor_y -= n;
}

uint32_t put_text(const uint8_t *s){
	uint16_t i;
	uint16_t *vram = (uint16_t*)0xb8000;

	if(graphic)
		return 0;

	for(i=0; s[i]; i++){
		vram[cursor_y*0x28 + cursor_x] = 0x0700 + s[i];
		cursor_x++;
		if(cursor_x >= 0x28 || !(s[i]^0x0a)){
			cursor_x = 0;
			cursor_y++;
		}

		if(cursor_y >= 0x19)
			scroll_page(1);
	}

	return i;
}

void set_graphicmode(void){
	graphic = true;

	cli();
        out_port(0x3c4, 2);
        out_port(0x3c5, 0x4);
        out_port(0x3c4, 4);
        out_port(0x3c5, 0x6);

	out_port(0x3ce, 5);
	out_port(0x3cf, 0x0);
	out_port(0x3ce, 6);
	out_port(0x3cf, 0x5);
	sti();
}

