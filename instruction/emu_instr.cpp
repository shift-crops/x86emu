#include "instruction/emu.hpp"
#include "instruction/instruction.hpp"
#include "emulator/exception.hpp"
#include "emulator/descriptor.hpp"

uint8_t EmuInstr::type_descriptor(uint16_t sel){
	uint32_t gdt_base;
	uint16_t gdt_limit;
	Descriptor desc;

	gdt_base = EMU->get_dtreg_base(GDTR);
	gdt_limit = EMU->get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	EMU->read_data(&desc, gdt_base + sel, sizeof(Descriptor));
	if(desc.S){
		if(((SegDesc*)&desc)->type.segc)
			return TYPE_CODE;
		else
			return TYPE_DATA;
	}
	else if(desc.type == 3)
		return TYPE_TSS;

	return desc.type;
}

void EmuInstr::set_ldtr(uint16_t sel){
	uint32_t gdt_base, base;
	uint16_t gdt_limit, limit;
	LDTDesc ldt;

	gdt_base = EMU->get_dtreg_base(GDTR);
	gdt_limit = EMU->get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	EMU->read_data(&ldt, gdt_base + sel, sizeof(LDTDesc));

	base = (ldt.base_h << 24) + (ldt.base_m << 16) + ldt.base_l;
	limit = (ldt.limit_h << 16) + ldt.limit_l;
	EMU->set_dtreg(LDTR, sel, base, limit); 
}

void EmuInstr::set_tr(uint16_t sel){
	uint32_t gdt_base, base;
	uint16_t gdt_limit, limit;
	TSSDesc tssdesc;

	gdt_base = EMU->get_dtreg_base(GDTR);
	gdt_limit = EMU->get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	EMU->read_data(&tssdesc, gdt_base + sel, sizeof(TSSDesc));
	EXCEPTION(EXP_GP, tssdesc.type != TYPE_TSS);

	base = (tssdesc.base_h << 24) + (tssdesc.base_m << 16) + tssdesc.base_l;
	limit = (tssdesc.limit_h << 16) + tssdesc.limit_l;

	EMU->set_dtreg(TR, sel, base, limit); 
}

void EmuInstr::switch_task(uint16_t sel){
	uint32_t base;
	uint16_t prev, limit;
	TSS old_tss, new_tss;  

	prev = EMU->get_dtreg_selector(TR);
	base = EMU->get_dtreg_base(TR);
	limit = EMU->get_dtreg_limit(TR);
	EXCEPTION(EXP_GP, limit < sizeof(TSS)-1);

	EMU->read_data(&old_tss, base, sizeof(TSS));
	old_tss.cr3 = EMU->get_crn(3);
	old_tss.eip = EMU->get_eip();
	old_tss.eflags = EMU->get_eflags();
	old_tss.eax = EMU->get_gpreg(EAX);
	old_tss.ecx = EMU->get_gpreg(ECX);
	old_tss.edx = EMU->get_gpreg(EDX);
	old_tss.ebx = EMU->get_gpreg(EBX);
	old_tss.esp = EMU->get_gpreg(ESP);
	old_tss.ebp = EMU->get_gpreg(EBP);
	old_tss.esi = EMU->get_gpreg(ESI);
	old_tss.edi = EMU->get_gpreg(EDI);
	old_tss.es = EMU->get_segment(ES);
	old_tss.cs = EMU->get_segment(CS);
	old_tss.ss = EMU->get_segment(SS);
	old_tss.ds = EMU->get_segment(DS);
	old_tss.fs = EMU->get_segment(FS);
	old_tss.gs = EMU->get_segment(GS);
	old_tss.ldtr = EMU->get_dtreg_selector(LDTR);
	EMU->write_data(base, &old_tss, sizeof(TSS));

	set_tr(sel);

	base = EMU->get_dtreg_base(TR);
	limit = EMU->get_dtreg_limit(TR);
	EXCEPTION(EXP_GP, limit < sizeof(TSS)-1);

	EMU->read_data(&new_tss, base, sizeof(TSS));
	new_tss.prev_sel = prev;
	EMU->write_data(base, &new_tss, sizeof(TSS));
	EMU->set_crn(3, new_tss.cr3);
	EMU->set_eip(new_tss.eip);
	EMU->set_eflags(new_tss.eflags);
	EMU->set_gpreg(EAX, new_tss.eax);
	EMU->set_gpreg(ECX, new_tss.ecx);
	EMU->set_gpreg(EDX, new_tss.edx);
	EMU->set_gpreg(EBX, new_tss.ebx);
	EMU->set_gpreg(ESP, new_tss.esp);
	EMU->set_gpreg(EBP, new_tss.ebp);
	EMU->set_gpreg(ESI, new_tss.esi);
	EMU->set_gpreg(EDI, new_tss.edi);
	EMU->set_segment(ES, new_tss.es);
	EMU->set_segment(CS, new_tss.cs);
	EMU->set_segment(SS, new_tss.ss);
	EMU->set_segment(DS, new_tss.ds);
	EMU->set_segment(FS, new_tss.fs);
	EMU->set_segment(GS, new_tss.gs);
	set_ldtr(new_tss.ldtr);
}

void EmuInstr::jmpf(uint16_t sel, uint32_t eip){
	if(EMU->is_protected()){
		switch(type_descriptor(sel)){
			case TYPE_CODE:
				goto jmp;
			case TYPE_TSS:
				switch_task(sel);
				return;
		}
	}

jmp:
	INFO(2, "cs = 0x%04x, eip = 0x%08x", sel, eip);
	EMU->set_segment(CS, sel);
	EMU->set_eip(eip);
}

void EmuInstr::callf(uint16_t sel, uint32_t eip){
/*
	if(EMU->is_protected()){
		switch(type_descriptor(sel)){
			case TYPE_CODE:
				goto jmp;
			case TYPE_TSS:
				switch_task(sel);
				return;
		}
	}

jmp:
*/
	SGRegister cs;
	uint8_t RPL;

	cs.raw = EMU->get_segment(CS);
	RPL = sel & 3;

	if(cs.RPL ^ RPL){
		EXCEPTION(EXP_GP, RPL < cs.RPL);
		EMU->push32(EMU->get_segment(SS));
		EMU->push32(EMU->get_gpreg(ESP));
	}

	EMU->push32(cs.raw);
	EMU->push32(EMU->get_eip());

	EMU->set_segment(CS, sel);
	EMU->set_eip(eip);
}

void EmuInstr::retf(void){
	SGRegister cs;
	uint8_t CPL;

	CPL = EMU->get_segment(CS) & 3;

	EMU->set_eip(EMU->pop32());
	cs.raw = EMU->pop32();

	if(cs.RPL ^ CPL){
		uint32_t esp;
		uint16_t ss;

		esp = EMU->pop32();
		ss = EMU->pop32();
		EMU->set_gpreg(ESP, esp);
		EMU->set_segment(SS, ss);
	}

	EMU->set_segment(CS, cs.raw);
}

void EmuInstr::iret(void){
	if(is_mode32()){
		SGRegister cs;
		uint8_t CPL;
		EFLAGS eflags;

                CPL = EMU->get_segment(CS) & 3;

		EMU->set_eip(EMU->pop32());
		cs.raw = EMU->pop32();
		eflags.reg32 = EMU->pop32();
		EMU->set_eflags(eflags.reg32);

		if(eflags.NT){
			uint32_t base;
			TSS tss;

			base = EMU->get_dtreg_base(TR);
			EMU->read_data(&tss, base, sizeof(TSS));
			switch_task(tss.prev_sel);
		}
		else{
			if(cs.RPL > CPL){
				uint32_t esp;
				uint16_t ss;

				esp = EMU->pop32();
				ss = EMU->pop32();
				EMU->set_gpreg(ESP, esp);
				EMU->set_segment(SS, ss);
			}
		}

		EMU->set_segment(CS, cs.raw);
		INFO(4, "iret (EIP : 0x%08x, CS : 0x%04x)", EMU->get_eip(), EMU->get_segment(CS));
	}
	else{
		uint16_t cs;
		EMU->set_ip(EMU->pop16());
		cs = EMU->pop16();
		EMU->set_flags(EMU->pop16());

		EMU->set_segment(CS, cs);
		INFO(4, "iret (IP : 0x%04x, CS : 0x%04x)", EMU->get_ip(), EMU->get_segment(CS));
	}
}

bool EmuInstr::chk_ring(uint8_t DPL){
	uint8_t CPL;
	CPL = EMU->get_segment(CS) & 3;

	return CPL<=DPL;
}
