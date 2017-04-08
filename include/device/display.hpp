#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "common.hpp"
#include "dev_io.hpp"
#include "vram.hpp"

enum vram_t {MODE_GRAPHIC, MODE_TEXT_MONO, MODE_TEXT_COLOR};

//class Display : public PortIO {
class Display {
	public:
		uint16_t size_x;
		uint16_t size_y;
		uint8_t zoom;

	private:
		VRAM g_vram;
		VRAM tm_vram;
		VRAM tc_vram;
		vram_t mode = MODE_GRAPHIC;
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
		Display() { image = NULL; };
		~Display() { if(image) delete[] image; };
		void resize(uint16_t x, uint16_t y, uint8_t z);
		VRAM* get_vram(vram_t m);
		bool need_refresh(void) { return get_vram(mode)->need_refresh(); };
		uint8_t *get_image(void);
};

#endif
