#include <map>
#include "emulator/interrupt.hpp"
#include "emulator/exception.hpp"
#include "emulator/descriptor.hpp"

void Interrupt::hundle_interrupt(void){
	std::pair<uint8_t, bool> intr;
	uint8_t n;
	uint16_t cs;
	bool hard;

	if(intr_q.empty())
		return;

	intr = intr_q.front();
	intr_q.pop();
	n = intr.first;
	hard = intr.second;

	if(is_protected()){
		IntGateDesc idt;
		uint32_t idt_base;
		uint16_t idt_limit, idt_offset;
		uint8_t CPL, RPL;

		CPL = get_segment(CS) & 3;

		idt_base = get_dtreg_base(IDTR);
		idt_limit = get_dtreg_limit(IDTR);
		idt_offset = n<<3;

		EXCEPTION(EXP_GP, idt_offset > idt_limit);

		read_data(&idt, idt_base + idt_offset, sizeof(IntGateDesc));
		RPL = ((SGRegister*)&(idt.seg_sel))->RPL;

		INFO(4, "int 0x%02x [CPL : %d, DPL : %d RPL : %d] (EIP : 0x%04x, CS : 0x%04x)"
				, n, CPL, idt.DPL, RPL, (idt.offset_h << 16) + idt.offset_l, idt.seg_sel);

		EXCEPTION(EXP_NP, !idt.P);
		EXCEPTION(EXP_GP, CPL < RPL);
		EXCEPTION(EXP_GP, !hard && CPL > idt.DPL);

		cs = get_segment(CS);
		set_segment(CS, idt.seg_sel);
		save_regs(CPL ^ RPL, cs);
		set_eip((idt.offset_h << 16) + idt.offset_l);

		if(idt.type == TYPE_INTERRUPT)
			set_interrupt(false);
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

		cs = get_segment(CS);
		set_segment(CS, ivt.segment);
		save_regs(false, cs);
		set_ip(ivt.offset);

		//set_interrupt(false);
		INFO(4, "int 0x%02x (IP : 0x%04x, CS : 0x%04x)", n, ivt.offset, ivt.segment);
	}
}

bool Interrupt::chk_irq(void){
	int8_t n_intr;

	if(!is_interrupt())
		return false;
	if(!pic_m || !pic_m->chk_intreq())
		return false;

	n_intr = pic_m->get_nintr();
        if(n_intr < 0)
		n_intr = pic_s->get_nintr();
	queue_interrupt(n_intr, true);
	return true;
}

void Interrupt::save_regs(bool chpl, uint16_t cs){
	if(is_protected()){
		if(chpl){
			uint32_t base, limit, esp;
			uint16_t ss;
			TSS tss;

			base = get_dtreg_base(TR);
			limit = get_dtreg_limit(TR);
			EXCEPTION(EXP_TS, limit < sizeof(TSS)-1);

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
		push32(cs);
		push32(get_eip());
	}
	else{
		push16(get_flags());
		push16(cs);
		push16(get_ip());
	}
}

