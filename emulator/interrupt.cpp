#include <map>
#include "emulator/interrupt.hpp"
#include "emulator/access.hpp"
#include "emulator/exception.hpp"
#include "emulator/structs.hpp"
#include "hardware/processor.hpp"
#include "hardware/eflags.hpp"
#include "hardware/cr.hpp"
#include "hardware/memory.hpp"

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
		SGRegister cs;
		uint8_t CPL, RPL;

		*((uint16_t*)&cs) = get_sgreg(CS);
		CPL = cs.RPL;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = n<<3;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);

		read_data(&idt, idt_base + idt_offset, sizeof(INTDescriptor));
		RPL = ((SGRegister*)&(idt.selector))->RPL;

		EXCEPTION(EXP_NP, !idt.P);
		EXCEPTION(EXP_GP, CPL < RPL);
		EXCEPTION(EXP_GP, !hard && CPL > idt.DPL);

		save_regs(CPL ^ RPL);
		set_eip((idt.offset_h << 16) + idt.offset_l);
		set_sgreg(CS, idt.selector);

		if(idt.type == TYPE_INTERRUPT)
			set_interruptable(false);

		INFO("int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)"
				, n, CPL, idt.DPL, RPL, (idt.offset_h << 16) + idt.offset_l, idt.selector);
	}
	else{
		IVT ivt;
		ivt.raw = read_mem32(n*4);

		save_regs(false);
		set_ip(ivt.offset);
		set_sgreg(CS, ivt.segment);

		set_interruptable(false);
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
			INFO("restore_regs (CS : 0x%04x->0x%04x, ESP : 0x%08x->0x%08x, SS : 0x%04x->0x%04x)"
					, cs0.raw, cs.raw, get_gpreg(ESP), esp, get_sgreg(SS), ss);
			set_gpreg(ESP, esp);
			set_sgreg(SS, ss);
		}
	}
	else{
		set_ip(pop16());
		set_sgreg(CS, pop16());
		set_flags(pop16());
	}
}

