#include <stdint.h>
#include "emulator/pic.hpp"

PIC::PIC(PIC* master) {
	pic_m = master;
	for(int i=0; i<MAX_IRQ; i++)
		irq[i] = NULL;
	irr = 0;
	isr = 0;
	/*
	ic[0] = &ic1.raw;
	ic[1] = &ic2.raw;
	ic[2] = &ic3.raw;
	ic[3] = &ic4.raw;
	*/
	init_icn = 1;
};

int8_t PIC::get_nintr(void){
	uint8_t iva;
	int i;

	for(i=0; i<MAX_IRQ && !((irr>>i)&1); i++);
	if(i == MAX_IRQ)
		return -1;
	//INFO("IRQ %d", i);

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
