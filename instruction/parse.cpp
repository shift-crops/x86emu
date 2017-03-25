#include <string.h>
#include "instruction/instruction.hpp"

bool ParseInstr::parse(void){
	uint8_t code;

	memset(&instr, 0, sizeof(instr));

	code = get_emu()->get_code8(0);
	UPDATE_EIP(1);

	if(code == 0x66 || code == 0x67){
		if(code == 0x66 && get_emu()->get_code8(0) == 0x67)
			UPDATE_EIP(1);
		return false;
	}

	OPCODE = code;

	DEBUG_MSG("EIP:0x%02x opcode:%02x ", GET_EIP()-1, OPCODE);

	if(chk[OPCODE].modrm)
		parse_modrm_sib_disp();

	if(chk[OPCODE].imm32){
		IMM32 = get_emu()->get_code32(0);
		DEBUG_MSG("imm32:%d", IMM32);
		UPDATE_EIP(4);
	}
	else if(chk[OPCODE].imm16){
		IMM16 = get_emu()->get_code16(0);
		DEBUG_MSG("imm16:%d", IMM16);
		UPDATE_EIP(2);
	}
	else if(chk[OPCODE].imm8){
		IMM8 = (int8_t)get_emu()->get_code8(0);
		DEBUG_MSG("imm8_s:%d", IMM8);
		UPDATE_EIP(1);
	}

	DEBUG_MSG("\n");

	return true;
}

void ParseInstr::parse_modrm_sib_disp(void){
	_MODRM = get_emu()->get_code8(0);
	UPDATE_EIP(1);

	DEBUG_MSG("[mod:%d reg:%d rm:%d] ", MOD, REG, RM);

	if (MOD != 3 && RM == 4) {
		_SIB = get_emu()->get_code8(0);
		UPDATE_EIP(1);
		DEBUG_MSG("[scale:%d index:%d base:%d] ", SCALE, INDEX, BASE);
	}

	if ((MOD == 0 && RM == 5) || MOD == 2) {
		DISP32 = get_emu()->get_code32(0);
		UPDATE_EIP(4);
		DEBUG_MSG("disp32:%d ", DISP32);
	}
	else if (MOD == 1) {
		DISP8 = (int8_t)get_emu()->get_code8(0);
		UPDATE_EIP(1);
		DEBUG_MSG("disp8:%d ", DISP8);
	}
}
