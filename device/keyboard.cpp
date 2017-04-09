#include "device/keyboard.hpp"

uint8_t Keyboard::in8(uint16_t addr){
	switch(addr){
		case 0x60:	return read_outbuf();
		case 0x64:	return kcsr.raw;
	}
	return -1;
}

void Keyboard::out8(uint16_t addr, uint8_t v){
	switch(addr){
		case 0x60:
			kcsr.F1 = 0;
			break;
		case 0x64:
			kcsr.F1 = 1;
			break;
	}
	command(v);
}

void Keyboard::command(uint8_t v){
	if(!kcsr.ST6){
		if(kcsr.F1){
			switch(v){
				case 0xa7:
					ccb->ME = 0;	// enable
					return;
				case 0xa8:
					ccb->ME = 1;	// diable
					return;
				case 0xad:
					ccb->KE = 0;	// enable
					return;
				case 0xae:
					ccb->KE = 1;	// diable
					return;
				default:
					if(v < 0x40){
						write_outbuf(controller_ram[v%0x20]);
						return;
					}
			}
		}
		else{
		}

		mode = v;
		kcsr.ST6 = 1;
	}
	else{
		if(kcsr.F1){
		}
		else{
			switch(mode){
				case 0xd2:
					send_code(v);
					break;
				case 0xd3:
					mouse->send_code(v);
					break;
				case 0xd4:
					mouse->command(v);
					break;
				default:
					if(mode >= 0x40 && mode < 0x80)
						controller_ram[(mode-0x40)%0x20] = v;
			}
		}
		kcsr.ST6 = 0;
	}
}

void Keyboard::write_outbuf(uint8_t v){
	kcsr.OBF = 1;
	out_buf = v;
	if(ccb->KIE)
		intr = true;
}

uint8_t Keyboard::read_outbuf(void){
	kcsr.OBF = 0;
	return out_buf;
}

void Keyboard::send_code(uint32_t scancode){
	if(kcsr.OBF)
		return;

	if(!ccb->KE) 	// enable
		write_outbuf(scancode);
}
