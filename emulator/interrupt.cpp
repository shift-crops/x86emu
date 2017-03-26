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
		INFO("int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt->offset, ivt->segment);
	}
}

void Interrupt::iret(void){
	restore_regs();
	if(is_protected())
		INFO("iret (EIP : 0x%08x, CS : 0x%04x)", get_eip(), get_segreg(CS));
	else
		INFO("iret (IP : 0x%04x, CS : 0x%04x)", get_ip(), get_segreg(CS));
}

void Interrupt::save_regs(void){
	if(is_protected()){
		push32(get_eflags());
		push32(get_segreg(CS));
		push32(get_eip());
	}
	else{
		push16(get_flags());
		push16(get_segreg(CS));
		push16(get_ip());
	}
}

void Interrupt::restore_regs(void){
	if(is_protected()){
		set_eip(pop32());
		set_segreg(CS, pop32());
		set_eflags(pop32());
	}
	else{
		set_ip(pop16());
		set_segreg(CS, pop16());
		set_flags(pop16());
	}
}
