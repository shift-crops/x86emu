#include "device/vga.hpp"
#define chk_regidx(n)	do{ if(n>sizeof(regs))ERROR("register index out of bound", n); if(!regs[n])ERROR("not implemented"); }while(0);

void VGA::get_windowsize(uint16_t *x, uint16_t *y){
	crt.get_windowsize(x, y);
}

void VGA::rgb_image(uint8_t *buffer, uint32_t size){
	gmode_t mode = gc.graphic_mode();

	for(uint32_t i=0; i<size; i++){
		uint8_t attr_idx, dac_idx;
		uint32_t rgb;

		attr_idx = mode^MODE_TEXT ? gc.attr_index_graphic(i) : crt.attr_index_text(i);
		dac_idx  = mode^MODE_GRAPHIC256 ? attr.dac_index(attr_idx) : attr_idx;
		rgb      = dac.translate_rgb(dac_idx);

		*(buffer++) = rgb&0xff;
		*(buffer++) = (rgb>>8)&0xff;
		*(buffer++) = (rgb>>16)&0xff;
	}
}

uint8_t VGA::in8(uint16_t addr){
	return 0;
}

void VGA::out8(uint16_t addr, uint8_t v){
}

uint32_t VGA::read32(uint32_t offset){
	return 0;
}

uint16_t VGA::read16(uint32_t offset){
	return 0;
}

uint8_t VGA::read8(uint32_t offset){
	return 0;
}

void VGA::write32(uint32_t offset, uint32_t v){
	refresh = true;
}

void VGA::write16(uint32_t offset, uint16_t v){
	refresh = true;
}

void VGA::write8(uint32_t offset, uint8_t v){
	refresh = true;
	// TODO
	plane[2][offset] = v;
}

/* Sequencer */
uint8_t *VGA::Sequencer::get_font(uint8_t att){
	uint16_t font_ofst = 0;

	if(mem_mr.EM){
		uint8_t v = (att&0x8) ? cmsr.CMA : cmsr.CMB;
		font_ofst = v<4 ? v*2 : (v-4)*2 + 1;
	}

	return vga->plane[2] + font_ofst;
}

uint8_t VGA::Sequencer::in8(uint16_t addr){
	switch(addr){
		case 0x3c4:	return sar.raw;
		case 0x3c5:	return *regs[sar.INDX];
	}
	return -1;
}

void VGA::Sequencer::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x3c4:
			chk_regidx(v);
			sar.raw = v;
			break;
		case 0x3c5:
			*regs[sar.INDX] = v;
			break;
	}
}

/* CRT */
void VGA::CRT::get_windowsize(uint16_t *x, uint16_t *y){
	*x = 8 * hdeer.HDEE;
	*y = 8 * vdeer.VDEE;
}

uint8_t VGA::CRT::attr_index_text(uint32_t n){
	uint8_t idx, chr, att;
	uint8_t *font;
	uint8_t bits;
	uint16_t x, y;

	x = n % hdeer.HDEE;
	y = n / hdeer.HDEE;

	idx = y/(mslr.MSL+1)*(hdeer.HDEE/8) + x/8;
	chr = vga->plane[0][idx];
	att = vga->plane[1][idx];

	font = vga->seq.get_font(att);
	bits = *(font + chr*0x10 + y%(mslr.MSL+1));

	return (bits>>(x%8))&1 ? att&0x0f : (att&0xf0)>>4;
}

uint8_t VGA::CRT::in8(uint16_t addr){
	switch(addr){
		case 0x3b4:
		case 0x3d4:
			return crtcar.raw;
		case 0x3b5:
		case 0x3d5:
			return *regs[crtcar.INDX];
	}
	return -1;
}

void VGA::CRT::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x3b4:
		case 0x3d4:
			chk_regidx(v);
			crtcar.raw = v;
			break;
		case 0x3b5:
		case 0x3d5:
			*regs[crtcar.INDX] = v;
			break;
	}
}

/* GraphicController */
gmode_t VGA::GraphicController::graphic_mode(void){
	if(mr.GM){
		if(gmr._256CM)
			return MODE_GRAPHIC256;
		return MODE_GRAPHIC;
	}
	return MODE_TEXT;
}

uint8_t VGA::GraphicController::attr_index_graphic(uint32_t n){
	return vga->plane[2][n];
}

uint8_t VGA::GraphicController::in8(uint16_t addr){
	switch(addr){
		case 0x3ce:	return gcar.raw;
		case 0x3cf:	return *regs[gcar.INDX];
	}
	return -1;
}

void VGA::GraphicController::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x3ce:
			chk_regidx(v);
			gcar.raw = v;
			break;
		case 0x3cf:
			*regs[gcar.INDX] = v;
			break;
	}
}

/* Attribute */
uint8_t VGA::Attribute::dac_index(uint8_t index){
	uint8_t dac_idx;

	union {
		uint8_t raw;
		struct {
			uint8_t low : 4;
			uint8_t high : 2;
		};
	} ip_data;
	ip_data.raw = ipr[index&0xf].raw;

	if(amcr.GAM){
		dac_idx  = ip_data.low;
		dac_idx += (amcr.P54S ? csr.SC45 : ip_data.high) << 4;
		dac_idx += csr.SC67 << 6;
	}
	else
		dac_idx  = ip_data.low;

	return dac_idx;
}

uint8_t VGA::Attribute::in8(uint16_t addr){
	switch(addr){
		case 0x3c0:	return acar.raw;
		case 0x3c1:	return *regs[acar.INDX];
	}
	return -1;
}

void VGA::Attribute::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x3c0:
			chk_regidx(v);
			acar.raw = v;
			break;
		case 0x3c1:
			*regs[acar.INDX] = v;
			break;
	}
}

/* DAC */
uint32_t VGA::DAC::translate_rgb(uint8_t index){
	uint32_t rgb;

	// 6bit -> 8bit
	rgb  = clut[index].R<<0x02;
	rgb += clut[index].G<<0x0a;
	rgb += clut[index].B<<0x12;

	return rgb;
}

uint8_t VGA::DAC::in8(uint16_t addr){
	uint8_t v;

	switch(addr){
		case 0x3c6:	return pelmr.raw;
		case 0x3c7:	return dacsr.raw;
		case 0x3c9:
			v = clut[r_par.index].raw[progress++];
			if(progress==3){
				progress = 0;
				r_par.index++;
			}
			return v;
	}
	return -1;
}

void VGA::DAC::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x3c7:
			if(v>0xff)	ERROR("");
			r_par.raw = v;
			progress = 0;
			break;
		case 0x3c8:
			if(v>0xff)	ERROR("");
			w_par.raw = v;
			progress = 0;
			break;
		case 0x3c9:
			clut[w_par.index].raw[progress++] = v;
			if(progress==3){
				progress = 0;
				w_par.index++;
			}
			break;
	}
}
