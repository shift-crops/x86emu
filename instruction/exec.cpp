#include "instruction/instruction.hpp"

bool ExecInstr::exec(void){
        if(!instrfuncs[OPCODE]){
                ERROR("not implemented OPCODE 0x%02x", OPCODE);
		return false;
	}

        (this->*instrfuncs[OPCODE])();
	return true;
}

void ExecInstr::set_rm32(uint32_t value){
	if(MOD == 3)
		SET_GPREG(static_cast<reg32_t>(RM), value);
	else 
		WRITE_MEM32(calc_modrm(), value);
}

uint32_t ExecInstr::get_rm32(void){
	if(MOD == 3)
		return GET_GPREG(static_cast<reg32_t>(RM));
	else
		return READ_MEM32(calc_modrm());
}

void ExecInstr::set_r32(uint32_t value){
	SET_GPREG(static_cast<reg32_t>(REG), value);
}

uint32_t ExecInstr::get_r32(void){
	return GET_GPREG(static_cast<reg32_t>(REG));
}

void ExecInstr::set_rm16(uint16_t value){
	if(MOD == 3)
		SET_GPREG(static_cast<reg16_t>(RM), value);
	else 
		WRITE_MEM16(calc_modrm(), value);
}

uint16_t ExecInstr::get_rm16(void){
	if(MOD == 3)
		return GET_GPREG(static_cast<reg16_t>(RM));
	else
		return READ_MEM16(calc_modrm());
}

void ExecInstr::set_r16(uint16_t value){
	SET_GPREG(static_cast<reg16_t>(REG), value);
}

uint16_t ExecInstr::get_r16(void){
	return GET_GPREG(static_cast<reg16_t>(REG));
}

void ExecInstr::set_rm8(uint8_t value){
	if(MOD == 3)
		SET_GPREG(static_cast<reg8_t>(RM), value);
	else 
		WRITE_MEM8(calc_modrm(), value);
}

uint8_t ExecInstr::get_rm8(void){
	if(MOD == 3)
		return GET_GPREG(static_cast<reg8_t>(RM));
	else
		return READ_MEM8(calc_modrm());
}

void ExecInstr::set_r8(uint8_t value){
	SET_GPREG(static_cast<reg8_t>(REG), value);
}

uint8_t ExecInstr::get_r8(void){
	return GET_GPREG(static_cast<reg8_t>(REG));
}

void ExecInstr::set_sreg(uint16_t value){
	EMU->set_segreg(static_cast<segreg_t>(REG), value);
}

uint16_t ExecInstr::get_sreg(void){
	return EMU->get_segreg(static_cast<segreg_t>(REG));
}

uint32_t ExecInstr::calc_modrm(void){
	switch(MOD){
		case 0:
			if(RM == 4)
				return calc_sib();
			else if(RM == 5)
				return DISP32;
			else
    				return GET_GPREG(static_cast<reg32_t>(RM));
		case 1:
			if(RM == 4)
				return calc_sib() + DISP8;
			else
    				return GET_GPREG(static_cast<reg32_t>(RM)) + DISP8;
		case 2:
			if(RM == 4)
				return calc_sib() + DISP32;
			else
    				return GET_GPREG(static_cast<reg32_t>(RM)) + DISP32;
	}
	return -1;
}

uint32_t ExecInstr::calc_sib(void){
	if(BASE!=5 && INDEX!=4)
		return GET_GPREG(static_cast<reg32_t>(BASE)) + GET_GPREG(static_cast<reg32_t>(INDEX)) * (1<<SCALE);
	else if(BASE==4 && INDEX==4 && SCALE==0)
		return GET_GPREG(static_cast<reg32_t>(ESP));

	ERROR("not implemented SIB (base = %d, index = %d, scale = %d)\n", BASE, INDEX, SCALE);
	return -1;
}

