#include <string.h>
#include "instruction.hpp"

void ParseInstr::parse(void){
	memset(&instr, 0, sizeof(instr));

	OPCODE = emu->get_code8(0);
	emu->update_eip(1);

	DEBUG_MSG("EIP:0x%02x opcode:%02x ", emu->get_eip()-1, OPCODE);

	if(chk[OPCODE].modrm)
		parse_modrm_sib_disp();

	if(chk[OPCODE].imm32){
		IMM32 = emu->get_code32(0);
		DEBUG_MSG("imm32:%d", IMM32);
		emu->update_eip(4);
	}
	else if(chk[OPCODE].imm8){
		IMM8 = (int8_t)emu->get_code8(0);
		DEBUG_MSG("imm8_s:%d", IMM8);
		emu->update_eip(1);
	}

	DEBUG_MSG("\n");
}

void ParseInstr::parse_modrm_sib_disp(void){
	_MODRM = emu->get_code8(0);
	emu->update_eip(1);

	DEBUG_MSG("[mod:%d reg:%d rm:%d] ", MOD, REG, RM);

	if (MOD != 3 && RM == 4) {
		_SIB = emu->get_code8(0);
		emu->update_eip(1);
		DEBUG_MSG("[scale:%d index:%d base:%d] ", SCALE, INDEX, BASE);
	}

	if ((MOD == 0 && RM == 5) || MOD == 2) {
		DISP32 = emu->get_code32(0);
		emu->update_eip(4);
		DEBUG_MSG("disp32:%d ", DISP32);
	}
	else if (MOD == 1) {
		DISP8 = (int8_t)emu->get_code8(0);
		emu->update_eip(1);
		DEBUG_MSG("disp8:%d ", DISP8);
	}
}
