#include "device/picreg.hpp"
#include "emulator/pic.hpp"

uint8_t PICReg::in8(uint16_t addr){
	// TODO
	switch(addr&1){
		case 1:
			return ~pic->imr;
	}

	return 0;
}

void PICReg::out8(uint16_t addr, uint8_t v){
	switch(addr&1){
		case 0:
			set_command(v);
			break;
		case 1:
			if(pic->init_icn > 1)
				set_data(v);
			else
				pic->imr = ~v;
			break;

	}
}

void PICReg::set_command(uint8_t v){
	if(pic->init_icn){
		pic->ic1.raw = v;
		INFO("ic1 : 0x%04x", v);
		pic->init_icn++;
	}
	else{
		OCW2 ocw2;

		ocw2.raw = v;
		if(ocw2.EOI)
			pic->isr = 0;
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

