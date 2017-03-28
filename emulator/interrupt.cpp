#include "emulator/interrupt.hpp"
#include "emulator/access.hpp"
#include "emulator/exception.hpp"
#include "emulator/descriptor.hpp"
#include "hardware/processor.hpp"
#include "hardware/eflags.hpp"
#include "hardware/cr.hpp"
#include "hardware/memory.hpp"


struct TSS {
	uint32_t T : 1;
	uint32_t : 15;
	uint32_t io_base : 16;
	uint32_t selector : 16;
	uint32_t : 16;
	uint32_t gs : 16;
	uint32_t : 16;
	uint32_t fs : 16;
	uint32_t : 16;
	uint32_t ds : 16;
	uint32_t : 16;
	uint32_t ss : 16;
	uint32_t : 16;
	uint32_t cs : 16;
	uint32_t : 16;
	uint32_t es : 16;
	uint32_t : 16;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t eflags;
	uint32_t eip;
	uint32_t cr3;
	uint32_t ss2 : 16;
	uint32_t : 16;
	uint32_t esp2;
	uint32_t ss1 : 16;
	uint32_t : 16;
	uint32_t esp1;
	uint32_t ss0 : 16;
	uint32_t : 16;
	uint32_t esp0;
	uint32_t backlink : 16;
	uint32_t : 16;
};

Interrupt::Interrupt(){
}

void Interrupt::interrupt_hundle(uint8_t n, bool hd_exp){
	if(!is_interruptable())
		return;

	if(is_protected()){
		INTDescriptor idt;
		uint32_t idt_base;
		uint16_t idt_limit, idt_offset;
		SGRegister cs;
		uint8_t CPL, RPL;

		*((uint16_t*)&cs) = get_sgreg(CS);
		CPL = cs.RPL;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = n<<3;

		if(idt_offset > idt_limit)
			throw EXP_GP;

		read_data(&idt, idt_base + idt_offset, sizeof(INTDescriptor));
		RPL = ((SGRegister*)&(idt.selector))->RPL;

		if(!idt.P)
			throw EXP_NP;
		if(CPL < RPL)
			throw EXP_GP;
		if(!hd_exp && CPL > idt.DPL)
			throw EXP_GP;

		if(idt.type == TYPE_INTERRUPT)
			set_interruptable(false);

		save_regs(CPL ^ RPL);
		set_eip((idt.offset_h << 16) + idt.offset_l);
		set_sgreg(CS, idt.selector);
		INFO("int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)"
				, n, CPL, idt.DPL, RPL, (idt.offset_h << 16) + idt.offset_l, idt.selector);
	}
	else{
		IVT ivt;
		ivt.raw = read_mem32(n*4);

		set_interruptable(false);

		save_regs(false);
		set_ip(ivt.offset);
		set_sgreg(CS, ivt.segment);
		INFO("int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt.offset, ivt.segment);
	}
}

void Interrupt::iret(void){
	restore_regs();
	set_interruptable(true);

	if(is_protected())
		INFO("iret (EIP : 0x%08x, CS : 0x%04x)", get_eip(), get_sgreg(CS));
	else
		INFO("iret (IP : 0x%04x, CS : 0x%04x)", get_ip(), get_sgreg(CS));
}

void Interrupt::save_regs(bool chpl){
	if(is_protected()){
		if(chpl){
			uint32_t gdt_base, base, limit, esp;
			uint16_t gdt_limit, gdt_index, ss;
			TSSDescriptor tdt;
			TSS tss;

			gdt_index = get_tr();
			gdt_base = get_dtreg_base(GDTR);
			gdt_limit = get_dtreg_limit(GDTR);

			if(!gdt_index || gdt_index > gdt_limit)
				throw EXP_GP;

			read_data(&tdt, gdt_base + gdt_index, sizeof(TSSDescriptor));

			base = (tdt.base_h << 24) + (tdt.base_m << 16) + tdt.base_l;
			limit = (tdt.limit_h << 16) + tdt.limit_l;

			if(limit < sizeof(TSS))
				throw EXP_TS;

			read_data(&tss, base, sizeof(TSS));

			ss = get_sgreg(SS);
			esp = get_gpreg(ESP);
			set_sgreg(SS, tss.ss0);
			set_gpreg(ESP, tss.esp0);

			INFO("save_regs (ESP : 0x%08x->0x%08x, SS : 0x%04x->0x%04x)", esp, tss.esp0, ss, tss.ss0);
			push32(ss);
			push32(esp);
		}
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
		SGRegister cs0, cs;

		cs0.raw = get_sgreg(CS);
		set_eip(pop32());
		set_sgreg(CS, cs.raw = pop32());
		set_eflags(pop32());

		if(cs.RPL > cs0.RPL){
			uint32_t esp;
			uint16_t ss;

			esp = pop32();
			ss = pop32();
			set_gpreg(ESP, esp);
			set_sgreg(SS, ss);
			INFO("restore_regs (CS : 0x%04x->0x%04x, ESP : 0x%08x, SS : 0x%04x)", cs0.RPL, cs.RPL, esp, ss);
		}
	}
	else{
		set_ip(pop16());
		set_sgreg(CS, pop16());
		set_flags(pop16());
	}
}
