#include <string.h>
#include <map>
#include "instruction/instruction.hpp"

bool ParseInstr::parse(bool is_protected){
	memset(&instr, 0, sizeof(instr));

	if(!parse_prefix_opcode())
		return false;

	if(!chk.count(OPCODE)){
		DEBUG_MSG("\n");
		ERROR("");
		return true;
	}

	if(chk[OPCODE].modrm)
		parse_modrm_sib_disp(is_protected);

	if(chk[OPCODE].imm32){
		IMM32 = get_emu()->get_code32(0);
		DEBUG_MSG("imm32:%d ", IMM32);
		UPDATE_EIP(4);
	}
	else if(chk[OPCODE].imm16){
		IMM16 = get_emu()->get_code16(0);
		DEBUG_MSG("imm16:%d ", IMM16);
		UPDATE_EIP(2);
	}
	else if(chk[OPCODE].imm8){
		IMM8 = (int8_t)get_emu()->get_code8(0);
		DEBUG_MSG("imm8:%d ", IMM8);
		UPDATE_EIP(1);
	}

	if(chk[OPCODE].ptr16){
		PTR16 = get_emu()->get_code16(0);
		DEBUG_MSG("ptr16:%d", PTR16);
		UPDATE_EIP(2);
	}

	DEBUG_MSG("\n");

	return true;
}

bool ParseInstr::parse_prefix_opcode(void){
	uint8_t code;

	code = get_emu()->get_code8(0);
	UPDATE_EIP(1);

	// prefix
	switch(code){
		case 0x26:
			SEGMENT = ES;
			goto next;
		case 0x2e:
			SEGMENT = CS;
			goto next;
		case 0x36:
			SEGMENT = SS;
			goto next;
		case 0x3e:
			SEGMENT = DS;
			goto next;
		case 0x64:
			SEGMENT = FS;
			goto next;
		case 0x65:
			SEGMENT = GS;
next:			PREFIX = code;
			code = get_emu()->get_code8(0);
			UPDATE_EIP(1);
			break;
		case 0x66:
			// size
			if(get_emu()->get_code8(0) == 0x67)
				UPDATE_EIP(1);
		case 0x67:
			return false;
	}

	OPCODE = code;

	// two byte opcode
	if(OPCODE == 0x0f){
		OPCODE = (OPCODE << 8) + get_emu()->get_code8(0);
		UPDATE_EIP(1);
	}

	DEBUG_MSG("EIP:0x%02x opcode:%02x ", GET_EIP()-1, OPCODE);

	return true;
}

void ParseInstr::parse_modrm_sib_disp(bool is_protected){
	_MODRM = get_emu()->get_code8(0);
	UPDATE_EIP(1);

	DEBUG_MSG("[mod:%d reg:%d rm:%d] ", MOD, REG, RM);

	if(is_protected){
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
	else{
		if ((MOD == 0 && RM == 6) || MOD == 2) {
			DISP16 = get_emu()->get_code32(0);
			UPDATE_EIP(2);
			DEBUG_MSG("disp16:%d ", DISP16);
		}
		else if (MOD == 1) {
			DISP8 = (int8_t)get_emu()->get_code8(0);
			UPDATE_EIP(1);
			DEBUG_MSG("disp8:%d ", DISP8);
		}
	}
}
