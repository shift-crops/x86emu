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
		IDT idt;

                uint32_t idt_base;
                uint16_t idt_limit, idt_offset;

                idt_base = get_dtreg_base(IDTR);
                idt_limit = get_dtreg_limit(IDTR);
                idt_offset = n*0x8;

                if(idt_offset > idt_limit)
                        throw "idt_limit";

                read_data(&idt, idt_base + idt_offset, sizeof(IDT));

		set_eip((idt.offset_h << 16) + idt.offset_l);
		set_sgreg(CS, idt.selector);
	}
	else{
		IVT ivt;
		ivt.raw = read_mem32(n*4);

		set_ip(ivt.offset);
		set_sgreg(CS, ivt.segment);
		INFO("int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt.offset, ivt.segment);
	}
}

void Interrupt::iret(void){
	restore_regs();
	if(is_protected())
		INFO("iret (EIP : 0x%08x, CS : 0x%04x)", get_eip(), get_sgreg(CS));
	else
		INFO("iret (IP : 0x%04x, CS : 0x%04x)", get_ip(), get_sgreg(CS));
}

void Interrupt::save_regs(void){
	if(is_protected()){
		push32(get_eflags());
		push32(get_sgreg(CS));
		push32(get_eip());
	}
	else{
		push16(get_flags());
		push16(get_sgreg(CS));
		push16(get_ip());
	}
}

void Interrupt::restore_regs(void){
	if(is_protected()){
		set_eip(pop32());
		set_sgreg(CS, pop32());
		set_eflags(pop32());
	}
	else{
		set_ip(pop16());
		set_sgreg(CS, pop16());
		set_flags(pop16());
	}
}
