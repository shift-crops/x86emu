#include <stdint.h>
#include "device/pic.hpp"

PIC::PIC(PIC* master) {
	pic_m = master;
	for(int i=0; i<MAX_IRQ; i++)
		irq[i] = NULL;
	irr = 0;
	isr = 0;
	init_icn = 1;
};

int8_t PIC::get_nintr(void){
	uint8_t iva;
	int i;

	for(i=0; i<MAX_IRQ && !((irr>>i)&1); i++);
	if(i == MAX_IRQ)
		return -1;
	INFO("IRQ %d", i);

	if(ic4.AEOI)	isr |= 1<<i;
	irr ^= 1<<i;

	if(!ic1.SNGL){
		if(!pic_m && ic3.raw&(1<<i))			// master
			return -1;
		else if(pic_m && !pic_m->chk_m2s_pic(ic3.ID))	// slave
			ERROR("");
	}

	iva = ic4.PM ? ic2.IVA_x86 << 3 : ic1.IVA_l + (ic2.IVA_h << 3);
	return iva + i;
}

bool PIC::chk_intreq(void){
	int i;

	if(init_icn)
		return false;

	if(irr)
		return false;

	for(i=0; i<MAX_IRQ && !((imr>>i)&1 && irq[i] && irq[i]->chk_intreq()); i++);
	if(i == MAX_IRQ)
		return false;
	if((1<<i) < isr)
		return false;

	irr = 1 << i;
	return true;
}

/*************************************************************************/

uint8_t PIC::in8(uint16_t addr){
	// TODO
	switch(addr&1){
		case 1:
			return ~imr;
	}

	return 0;
}

void PIC::out8(uint16_t addr, uint8_t v){
	switch(addr&1){
		case 0:
			set_command(v);
			break;
		case 1:
			if(init_icn > 1)
				set_data(v);
			else
				imr = ~v;
			break;

	}
}

void PIC::set_command(uint8_t v){
	if(init_icn){
		ic1.raw = v;
		INFO("ic1 : 0x%04x", v);
		init_icn++;
	}
	else{
		OCW2 ocw2;

		ocw2.raw = v;
		if(ocw2.EOI)
			isr = 0;
		// TODO
	}
}

void PIC::set_data(uint8_t v){
	switch(init_icn++){
		case 2:
			ic2.raw = v;
			INFO("ic2 : 0x%04x", v);
			if(ic1.SNGL)
				goto done;
			break;
		case 3:
			ic3.raw = v;
			INFO("ic3 : 0x%04x", v);
			if(!ic1.IC4)
				goto done;
			break;
		case 4:
			ic4.raw = v;
			INFO("ic4 : 0x%04x", v);
		default:
			goto done;
	}

	return;
done:	
	init_icn = 0;
}

