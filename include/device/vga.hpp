#ifndef _VGA_H
#define _VGA_H

#include "common.hpp"
#include "dev_io.hpp"
#include "vram.hpp"

enum vram_t {MODE_GRAPHIC, MODE_TEXT_MONO, MODE_TEXT_COLOR};

//class VGA : public PortIO {
class VGA {
	private:
		VRAM g_vram;
		VRAM tm_vram;
		VRAM tc_vram;
		vram_t mode = MODE_GRAPHIC;
		uint16_t size_x;
		uint16_t size_y;
		uint8_t *image;

		uint8_t rgb_palette[16][3] = {
			{0x00, 0x00, 0x00}, 
			{0xff, 0x00, 0x00},
			{0x00, 0xff, 0x00},
			{0xff, 0xff, 0x00},
			{0x00, 0x00, 0xff},
			{0xff, 0x00, 0xff},
			{0x00, 0xff, 0xff},
			{0xff, 0xff, 0xff},
			{0xc6, 0xc6, 0xc6},
			{0x84, 0x00, 0x00},
			{0x00, 0x84, 0x00},
			{0x84, 0x84, 0x00},
			{0x00, 0x00, 0x84},
			{0x84, 0x00, 0x84},
			{0x00, 0x84, 0x84},
			{0x84, 0x84, 0x84}
		};

	public:
		VGA() { size_x=320; size_y=200; image = new uint8_t[320*200*3]; };
		~VGA() { if(image) delete[] image; };
		VRAM* get_vram(vram_t m);
		bool get_windowsize(uint16_t *x, uint16_t *y) {*x=size_x; *y=size_y; return false; };
		bool need_refresh(void) { return get_vram(mode)->need_refresh(); };
		uint8_t *get_image(void);
};

#endif
