#include <map>
#include "emulator/interrupt.hpp"
#include "emulator/exception.hpp"
#include "emulator/structs.hpp"

void Interrupt::hundle_interrupt(void){
	std::pair<uint8_t, bool> intr;
	uint8_t n;
	bool hard;

	if(intr_q.empty())
		return;

	intr = intr_q.front();
	intr_q.pop();
	n = intr.first;
	hard = intr.second;

	if(is_protected()){
		INTDescriptor idt;
		uint32_t idt_base;
		uint16_t idt_limit, idt_offset;
		uint8_t CPL, RPL;

		CPL = get_segment(CS) & 3;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = n<<3;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);

		read_data(&idt, idt_base + idt_offset, sizeof(INTDescriptor));
		RPL = ((SGRegister*)&(idt.selector))->RPL;

		INFO(4, "int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)"
				, n, CPL, idt.DPL, RPL, (idt.offset_h << 16) + idt.offset_l, idt.selector);

		EXCEPTION(EXP_NP, !idt.P);
		EXCEPTION(EXP_GP, CPL < RPL);
		EXCEPTION(EXP_GP, !hard && CPL > idt.DPL);

		save_regs(CPL ^ RPL);
		set_eip((idt.offset_h << 16) + idt.offset_l);
		set_segment(CS, idt.selector);

		if(idt.type == TYPE_INTERRUPT)
			set_interruptable(false);
	}
	else{
		uint32_t idt_base;
		uint16_t idt_limit, idt_offset;
		IVT ivt;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = n<<2;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);
		ivt.raw = read_mem32(idt_base + idt_offset);

		save_regs(false);
		set_ip(ivt.offset);
		set_segment(CS, ivt.segment);

		//set_interruptable(false);
		INFO(4, "int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt.offset, ivt.segment);
	}
}

void Interrupt::iret(void){
	restore_regs();

	if(is_protected())
		INFO(4, "iret (EIP : 0x%08x, CS : 0x%04x)", get_eip(), get_segment(CS));
	else
		INFO(4, "iret (IP : 0x%04x, CS : 0x%04x)", get_ip(), get_segment(CS));
}

bool Interrupt::chk_irq(void){
	int8_t n_intr;

	if(!is_interruptable())
		return false;
	if(!pic_m || !pic_m->chk_intreq())
		return false;

	n_intr = pic_m->get_nintr();
        if(n_intr < 0)
		n_intr = pic_s->get_nintr();
	queue_interrupt(n_intr, true);
	return true;
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

			ss = get_segment(SS);
			esp = get_gpreg(ESP);
			set_segment(SS, tss.ss0);
			set_gpreg(ESP, tss.esp0);

			INFO(4, "save_regs (ESP : 0x%08x->0x%08x, SS : 0x%04x->0x%04x)", esp, tss.esp0, ss, tss.ss0);
			push32(ss);
			push32(esp);
		}
		push32(get_eflags());
		push32(get_segment(CS));
		push32(get_eip());
	}
	else{
		push16(get_flags());
		push16(get_segment(CS));
		push16(get_ip());
	}
}

void Interrupt::restore_regs(void){
	if(is_protected()){
		SGRegister cs0, cs;

		cs0.raw = get_segment(CS);
		set_eip(pop32());
		set_segment(CS, cs.raw = pop32());
		set_eflags(pop32());

		if(cs.RPL > cs0.RPL){
			uint32_t esp;
			uint16_t ss;

			esp = pop32();
			ss = pop32();
			INFO(4, "restore_regs (CS : 0x%04x->0x%04x, ESP : 0x%08x->0x%08x, SS : 0x%04x->0x%04x)"
					, cs0.raw, cs.raw, get_gpreg(ESP), esp, get_segment(SS), ss);
			set_gpreg(ESP, esp);
			set_segment(SS, ss);
		}
	}
	else{
		set_ip(pop16());
		set_segment(CS, pop16());
		set_flags(pop16());
	}
}

