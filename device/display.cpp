#include "device/display.hpp"

void Display::resize(uint16_t x, uint16_t y, uint8_t z){
	if(image)
		delete[] image;

	size_x = x;
	size_y = y;
	zoom = z;
	image = new uint8_t[x*y*3];
}

VRAM* Display::get_vram(vram_t m){
	switch(m){
		case MODE_GRAPHIC:	return &g_vram;
		case MODE_TEXT_MONO:	return &tm_vram;
		case MODE_TEXT_COLOR:	return &tc_vram;
	}
	return NULL;
}

uint8_t *Display::get_image(void){
	VRAM *vram = get_vram(mode);

	for(int i=0; i<320*200; i++){
		uint8_t *rgb = rgb_palette[vram->read8(i)];
		image[i*3] = rgb[0];
		image[i*3+1] = rgb[1];
		image[i*3+2] = rgb[2];
	}
	vram->refreshed();
	return image;
}
