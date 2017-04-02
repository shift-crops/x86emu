#include "device/picreg.hpp"
#include "emulator/pic.hpp"

uint8_t PICReg::in8(uint16_t addr){
	// TODO
	return 0;
}

void PICReg::out8(uint16_t addr, uint8_t v){
	if(!(addr & 1))
		set_command(v);
	else if(pic->init_icn > 1)
		set_data(v);
	else
		pic->imr = ~v;
}

void PICReg::set_command(uint8_t v){
	if(pic->init_icn){
		pic->ic1.raw = v;
		INFO("ic1 : 0x%04x", v);
		pic->init_icn++;
	}
	else{
		// TODO
	}
}

void PICReg::set_data(uint8_t v){
	switch(pic->init_icn++){
		case 2:
			pic->ic2.raw = v;
			INFO("ic2 : 0x%04x", v);
			if(pic->ic1.SNGL)
				goto done;
			break;
		case 3:
			pic->ic3.raw = v;
			INFO("ic3 : 0x%04x", v);
			if(!pic->ic1.IC4)
				goto done;
			break;
		case 4:
			pic->ic4.raw = v;
			INFO("ic4 : 0x%04x", v);
		default:
			goto done;
	}

	return;
done:	
	pic->init_icn = 0;
}

