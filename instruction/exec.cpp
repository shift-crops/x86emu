#include "instruction.hpp"

bool ExecInstr::exec(void){
        if(!instrfuncs[OPCODE]){
                ERROR("not implemented OPCODE 0x%02d", OPCODE);
		return false;
	}

        (this->*instrfuncs[OPCODE])();
	return true;
}

void ExecInstr::set_rm32(uint32_t value){
	if(MOD == 3)
		emu->set_gpreg(static_cast<reg32_t>(RM), value);
	else 
		emu->write_mem32(calc_modrm(), value);
}

uint32_t ExecInstr::get_rm32(void){
	if(MOD == 3)
		return emu->get_gpreg(static_cast<reg32_t>(RM));
	else
		return emu->read_mem32(calc_modrm());
}

void ExecInstr::set_r32(uint32_t value){
	emu->set_gpreg(static_cast<reg32_t>(REG), value);
}

uint32_t ExecInstr::get_r32(void){
	return emu->get_gpreg(static_cast<reg32_t>(REG));
}

void ExecInstr::set_rm8(uint8_t value){
	if(MOD == 3)
		emu->set_gpreg(static_cast<reg8_t>(RM), value);
	else 
		emu->write_mem8(calc_modrm(), value);
}

uint32_t ExecInstr::get_rm8(void){
	if(MOD == 3)
		return emu->get_gpreg(static_cast<reg8_t>(RM));
	else
		return emu->read_mem8(calc_modrm());
}

void ExecInstr::set_r8(uint8_t value){
	emu->set_gpreg(static_cast<reg8_t>(REG), value);
}

uint32_t ExecInstr::get_r8(void){
	return emu->get_gpreg(static_cast<reg8_t>(REG));
}

void ExecInstr::push32(uint32_t value){
	uint32_t esp;

	emu->update_gpreg(ESP, -4);
	esp = emu->get_gpreg(ESP);
	emu->write_mem32(esp, value);
	DEBUG_MSG("push(0x%08x) 0x%08x\n", esp, value);
}

uint32_t ExecInstr::pop32(void){
	uint32_t esp, value;

	esp = emu->get_gpreg(ESP);
	value = emu->read_mem32(esp);
	DEBUG_MSG("pop(0x%08x) 0x%08x\n", esp, value);
	emu->update_gpreg(ESP, 4);

	return value;
}

uint32_t ExecInstr::calc_modrm(void){
	switch(MOD){
		case 0:
			if(RM == 4)
				return calc_sib();
			else if(RM == 5)
				return DISP32;
			else
    				return emu->get_gpreg(static_cast<reg32_t>(RM));
		case 1:
			if(RM == 4)
				return calc_sib() + DISP8;
			else
    				return emu->get_gpreg(static_cast<reg32_t>(RM)) + DISP8;
		case 2:
			if(RM == 4)
				return calc_sib() + DISP32;
			else
    				return emu->get_gpreg(static_cast<reg32_t>(RM)) + DISP32;
	}
	return -1;
}

uint32_t ExecInstr::calc_sib(void){
	if(BASE!=5 && INDEX!=4)
		return emu->get_gpreg(static_cast<reg32_t>(BASE)) + emu->get_gpreg(static_cast<reg32_t>(INDEX)) * (1<<SCALE);
	else if(BASE==4 && INDEX==4 && SCALE==0)
		return emu->get_gpreg(static_cast<reg32_t>(ESP));

	ERROR("not implemented SIB (base = %d, index = %d, scale = %d)\n", BASE, INDEX, SCALE);
	return -1;
}

