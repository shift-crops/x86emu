#include "emulator/interrupt.hpp"
#include "emulator/access.hpp"
#include "hardware/processor.hpp"
#include "hardware/eflags.hpp"
#include "hardware/cr.hpp"
#include "hardware/memory.hpp"

Interrupt::Interrupt(){
}

void Interrupt::interrupt_hundle(uint8_t n){
	save_regs();

	if(is_protected()){
	}
	else{
		IVT *ivt;
		uint32_t raw = read_mem32(n*4);

		ivt = (IVT *)&raw;
		set_ip(ivt->offset);
		set_segreg(CS, ivt->segment);
		INFO("ip : %x, CS : %x", ivt->offset, ivt->segment);
	}
}

void Interrupt::iret(void){
	restore_regs();
}

void Interrupt::save_regs(void){
	push32(get_eflags());
	push32(get_segreg(CS));
	push32(get_eip());
}

void Interrupt::restore_regs(void){
	set_eip(pop32());
	set_segreg(CS, pop32());
	set_eflags(pop32());
}
