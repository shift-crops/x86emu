#include "device/vga.hpp"

VRAM* VGA::get_vram(vram_t m){
	switch(m){
		case MODE_GRAPHIC:	return &g_vram;
		case MODE_TEXT_MONO:	return &tm_vram;
		case MODE_TEXT_COLOR:	return &tc_vram;
	}
	return NULL;
}

uint8_t *VGA::get_image(void){
	VRAM *vram = get_vram(mode);

	for(int i=0; i<size_x*size_y; i++){
		uint8_t *rgb = rgb_palette[vram->read8(i)];
		image[i*3] = rgb[0];
		image[i*3+1] = rgb[1];
		image[i*3+2] = rgb[2];
	}
	vram->refreshed();
	return image;
}
