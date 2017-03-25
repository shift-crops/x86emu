#include <stdint.h>
#include "instruction/base.hpp"

#define instr32(f) ((instrfunc_t)&Instr32::f)

void Instr32::init_instr(void){
	int i;

	set_funcflag(0x00, instr32(add_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x01, instr32(add_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x02, instr32(add_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x03, instr32(add_r32_rm32) ,CHK_MODRM);
	//set_funcflag(0x04, instr32(add_al_imm8) ,CHK_IMM8);
	//set_funcflag(0x05, instr32(add_eax_imm32) ,CHK_IMM32);

	set_funcflag(0x08, instr32(or_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x09, instr32(or_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x0a, instr32(or_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x0b, instr32(or_r32_rm32) ,CHK_MODRM);
	//set_funcflag(0x0c, instr32(or_al_imm8) ,CHK_IMM8);
	//set_funcflag(0x0d, instr32(or_eax_imm32) ,CHK_IMM32);

	set_funcflag(0x20, instr32(and_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x21, instr32(and_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x22, instr32(and_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x23, instr32(and_r32_rm32) ,CHK_MODRM);
	//set_funcflag(0x24, instr32(and_al_imm8) ,CHK_IMM8);
	//set_funcflag(0x25, instr32(and_eax_imm32) ,CHK_IMM32);

	set_funcflag(0x28, instr32(sub_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x29, instr32(sub_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x2a, instr32(sub_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x2b, instr32(sub_r32_rm32) ,CHK_MODRM);
	//set_funcflag(0x2c, instr32(sub_al_imm8) ,CHK_IMM8);
	//set_funcflag(0x2d, instr32(sub_eax_imm32) ,CHK_IMM32);

	set_funcflag(0x30, instr32(xor_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x31, instr32(xor_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x32, instr32(xor_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x33, instr32(xor_r32_rm32) ,CHK_MODRM);
	//set_funcflag(0x34, instr32(xor_al_imm8) ,CHK_IMM8);
	//set_funcflag(0x35, instr32(xor_eax_imm32) ,CHK_IMM32);

	set_funcflag(0x38, instr32(cmp_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x39, instr32(cmp_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x3a, instr32(cmp_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x3b, instr32(cmp_r32_rm32) ,CHK_MODRM);
	set_funcflag(0x3c, instr32(cmp_al_imm8) ,CHK_IMM8);
	set_funcflag(0x3d, instr32(cmp_eax_imm32) ,CHK_IMM32);

	for (i=0; i<8; i++)	set_funcflag(0x40+i, instr32(inc_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x48+i, instr32(dec_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x50+i, instr32(push_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x58+i, instr32(pop_r32) ,0);

	set_funcflag(0x68, instr32(push_imm32) ,CHK_IMM32);
	//set_funcflag(0x69, instr32(imul_r32_rm32_imm32) ,CHK_MODRM|CHK_IMM32);
	set_funcflag(0x6a, instr32(push_imm8) ,CHK_IMM8);
	//set_funcflag(0x6b, instr32(imul_r32_rm32_imm8) ,CHK_MODRM|CHK_IMM8);

	set_funcflag(0x70, instr32(jo) ,CHK_IMM8);
	set_funcflag(0x71, instr32(jno) ,CHK_IMM8);
	set_funcflag(0x72, instr32(jb) ,CHK_IMM8);
	set_funcflag(0x73, instr32(jnb) ,CHK_IMM8);
	set_funcflag(0x74, instr32(jz) ,CHK_IMM8);
	set_funcflag(0x75, instr32(jnz) ,CHK_IMM8);
	set_funcflag(0x76, instr32(jbe) ,CHK_IMM8);
	set_funcflag(0x77, instr32(ja) ,CHK_IMM8);
	set_funcflag(0x78, instr32(js) ,CHK_IMM8);
	set_funcflag(0x79, instr32(jns) ,CHK_IMM8);
	set_funcflag(0x7a, instr32(jp) ,CHK_IMM8);
	set_funcflag(0x7b, instr32(jnp) ,CHK_IMM8);
	set_funcflag(0x7c, instr32(jl) ,CHK_IMM8);
	set_funcflag(0x7d, instr32(jnl) ,CHK_IMM8);
	set_funcflag(0x7e, instr32(jle) ,CHK_IMM8);
	set_funcflag(0x7f, instr32(jnle) ,CHK_IMM8);
	//set_funcflag(0x80, instr32(code_80) ,CHK_MODRM | CHK_IMM8);
	//set_funcflag(0x81, instr32(code_81) ,CHK_MODRM | CHK_IMM32);
	//set_funcflag(0x82, instr32(code_82) ,CHK_MODRM | CHK_IMM8);
	set_funcflag(0x83, instr32(code_83) ,CHK_MODRM | CHK_IMM8);
	set_funcflag(0x84, instr32(test_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x85, instr32(test_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x86, instr32(xchg_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x87, instr32(xchg_r32_rm32) ,CHK_MODRM);
	set_funcflag(0x88, instr32(mov_rm8_r8) ,CHK_MODRM);
	set_funcflag(0x89, instr32(mov_rm32_r32) ,CHK_MODRM);
	set_funcflag(0x8a, instr32(mov_r8_rm8) ,CHK_MODRM);
	set_funcflag(0x8b, instr32(mov_r32_rm32) ,CHK_MODRM);
	set_funcflag(0x8c, instr32(mov_rm32_sreg) ,CHK_MODRM);
	set_funcflag(0x8e, instr32(mov_sreg_rm16) ,CHK_MODRM);

	set_funcflag(0x90, instr32(nop) ,CHK_MODRM);
	for (i=1; i<8; i++)	set_funcflag(0x90+i, instr32(xchg_r32_eax) ,CHK_IMM32);

	for (i=0; i<8; i++)	set_funcflag(0xb0+i, instr32(mov_r8_imm8) ,CHK_IMM8);
	for (i=0; i<8; i++)	set_funcflag(0xb8+i, instr32(mov_r32_imm32) ,CHK_IMM32);

	set_funcflag(0xc3, instr32(ret) ,0);

	set_funcflag(0xc7, instr32(mov_rm32_imm32) ,CHK_MODRM | CHK_IMM32);

	set_funcflag(0xc9, instr32(leave) ,0);

	set_funcflag(0xcd, instr32(int_imm8) ,CHK_IMM8);

	set_funcflag(0xcf, instr32(iret) ,0);

	set_funcflag(0xe4, instr32(in_al_imm8), CHK_IMM8);
	set_funcflag(0xe5, instr32(in_eax_imm8), CHK_IMM8);
	set_funcflag(0xe6, instr32(out_imm8_al), CHK_IMM8);
	set_funcflag(0xe7, instr32(out_imm8_eax), CHK_IMM8);
	set_funcflag(0xe8, instr32(call_rel32) ,CHK_IMM32);
	set_funcflag(0xe9, instr32(jmp_rel32) ,CHK_IMM32);

	set_funcflag(0xeb, instr32(jmp_rel8) ,CHK_IMM8);
	set_funcflag(0xec, instr32(in_al_dx), 0);
	set_funcflag(0xed, instr32(in_eax_dx), 0);
	set_funcflag(0xee, instr32(out_dx_al), 0);
	set_funcflag(0xef, instr32(out_dx_eax), 0);

	set_funcflag(0xff, instr32(code_ff) ,CHK_MODRM);
}

void Instr32::add_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	set_rm32(rm32+r32);
	EFLAGS_UPDATE_ADD(rm32, r32);
}

void Instr32::add_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32+rm32);
	EFLAGS_UPDATE_ADD(r32, rm32);
}

void Instr32::or_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	set_rm32(rm32|r32);
	EFLAGS_UPDATE_OR(rm32, r32);
}

void Instr32::or_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32|rm32);
	EFLAGS_UPDATE_OR(r32, rm32);
}

void Instr32::and_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	set_rm32(rm32&r32);
	EFLAGS_UPDATE_AND(rm32, r32);
}

void Instr32::and_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32|rm32);
	EFLAGS_UPDATE_AND(r32, rm32);
}

void Instr32::sub_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	set_rm32(rm32-r32);
	EFLAGS_UPDATE_SUB(rm32, r32);
}

void Instr32::sub_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32-rm32);
	EFLAGS_UPDATE_SUB(r32, rm32);
}

void Instr32::xor_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	set_rm32(rm32^r32);
}

void Instr32::xor_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32^rm32);
}

void Instr32::cmp_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	EFLAGS_UPDATE_SUB(rm32, r32);
}

void Instr32::cmp_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	EFLAGS_UPDATE_SUB(r32, rm32);
}

void Instr32::cmp_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	EFLAGS_UPDATE_SUB(eax, IMM32);
}

void Instr32::inc_r32(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	UPDATE_GPREG(static_cast<reg32_t>(reg), 1);
}

void Instr32::dec_r32(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	UPDATE_GPREG(static_cast<reg32_t>(reg), -1);
}

void Instr32::push_r32(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	PUSH32(GET_GPREG(static_cast<reg32_t>(reg)));
}

void Instr32::pop_r32(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	SET_GPREG(static_cast<reg32_t>(reg), POP32());
}

void Instr32::push_imm32(void){
	PUSH32(IMM32);
}

void Instr32::push_imm8(void){
	PUSH32(IMM8);
}

/*
void Instr32::code_80(void){
	switch(REG){
		case 0:	add_rm8_imm8();		break;
		case 1:	or_rm8_imm8();		break;
		case 2:	adc_rm8_imm8();		break;
		case 3:	sbb_rm8_imm8();		break;
		case 4:	and_rm8_imm8();		break;
		case 5:	sub_rm8_imm8();		break;
		case 6:	xor_rm8_imm8();		break;
		case 7:	cmp_rm8_imm8();		break;
		default:
			ERROR("not implemented: 0x80 /%d\n", REG);
	}
}

void Instr32::code_81(void){
	switch(REG){
		case 0:	add_rm32_imm32();	break;
		case 1:	or_rm32_imm32();	break;
		case 2:	adc_rm32_imm32();	break;
		case 3:	sbb_rm32_imm32();	break;
		case 4:	and_rm32_imm32();	break;
		case 5:	sub_rm32_imm32();	break;
		case 6:	xor_rm32_imm32();	break;
		case 7:	cmp_rm32_imm32();	break;
		default:
			ERROR("not implemented: 0x81 /%d\n", REG);
	}
}

void Instr32::code_82(void){
	code_80();
}
*/

void Instr32::code_83(void){
	switch(REG){
		case 0:	add_rm32_imm8();	break;
		case 1:	or_rm32_imm8();		break;
		case 2:	adc_rm32_imm8();	break;
		case 3:	sbb_rm32_imm8();	break;
		case 4:	and_rm32_imm8();	break;
		case 5:	sub_rm32_imm8();	break;
		case 6:	xor_rm32_imm8();	break;
		case 7:	cmp_rm32_imm8();	break;
		default:
			ERROR("not implemented: 0x83 /%d\n", REG);
	}
}

void Instr32::test_rm32_r32(void){
	uint32_t rm32, r32;

	rm32 = get_rm32();
	r32 = get_r32();
	EFLAGS_UPDATE_AND(rm32, r32);
}

void Instr32::xchg_r32_rm32(void){
	uint32_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(rm32);
	set_rm32(r32);
}

void Instr32::mov_rm32_r32(void){
	uint32_t r32;

	r32 = get_r32();
	set_rm32(r32);
}

void Instr32::mov_r32_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_r32(rm32);
}

void Instr32::mov_rm32_sreg(void){
	uint16_t sreg;

	sreg = get_sreg();
	set_rm32(sreg);
}

void Instr32::xchg_r32_eax(void){
	uint32_t r32, eax;

	r32 = get_r32();
	eax = GET_GPREG(EAX);
	set_r32(eax);
	SET_GPREG(EAX, r32);
}

void Instr32::mov_r32_imm32(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	SET_GPREG(static_cast<reg32_t>(reg), IMM32);
}

void Instr32::ret(void){
	uint32_t addr;

	addr = POP32();
	SET_EIP(addr);
}

void Instr32::mov_rm32_imm32(void){
	set_rm32(IMM32);
}

void Instr32::leave(void){
	uint32_t ebp;

	ebp = GET_GPREG(EBP);
	SET_GPREG(ESP, ebp);
	SET_GPREG(EBP, POP32());
}

void Instr32::in_eax_imm8(void){
	SET_GPREG(EAX, EMU->in_io32(IMM8));
}

void Instr32::out_imm8_eax(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	EMU->out_io32(IMM8, eax);
}

void Instr32::call_rel32(void){
	PUSH32(GET_EIP());
	UPDATE_EIP(IMM32);
}

void Instr32::jmp_rel32(void){
	UPDATE_EIP(IMM32);
}

void Instr32::in_eax_dx(void){
	uint16_t dx;

	dx = GET_GPREG(DX);
	SET_GPREG(EAX, EMU->in_io32(dx));
}

void Instr32::out_dx_eax(void){
	uint16_t dx;
	uint32_t eax;

	dx = GET_GPREG(DX);
	eax = GET_GPREG(EAX);
	EMU->out_io32(dx, eax);
}

void Instr32::code_ff(void){
	switch(REG){
		case 0:
			inc_rm32();
			break;
		case 1:
			dec_rm32();
			break;
		case 2:
			call_rm32();
			break;
		case 4:
			jmp_rm32();
			break;
		case 6:
			push_rm32();
			break;
		default:
			ERROR("not implemented: 0xff /%d\n", REG);
	}
}

/******************************************************************/

void Instr32::add_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32+IMM8);
	EFLAGS_UPDATE_ADD(rm32, IMM8);
}

void Instr32::or_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32|IMM8);
	EFLAGS_UPDATE_OR(rm32, IMM8);
}

void Instr32::adc_rm32_imm8(void){
	uint32_t rm32;
	uint8_t cf;

	rm32 = get_rm32();
	cf = EFLAGS_CF;
	set_rm32(rm32+IMM8+cf);
	EFLAGS_UPDATE_ADD(rm32, IMM8+cf);
}

void Instr32::sbb_rm32_imm8(void){
	uint32_t rm32;
	uint8_t cf;

	rm32 = get_rm32();
	cf = EFLAGS_CF;
	set_rm32(rm32-IMM8-cf);
	EFLAGS_UPDATE_SUB(rm32, IMM8+cf);
}

void Instr32::and_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32&IMM8);
	EFLAGS_UPDATE_AND(rm32, IMM8);
}

void Instr32::sub_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32-IMM8);
	EFLAGS_UPDATE_SUB(rm32, IMM8);
}

void Instr32::xor_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32^IMM8);
}

void Instr32::cmp_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	EFLAGS_UPDATE_SUB(rm32, IMM8);
}

/******************************************************************/

void Instr32::inc_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32+1);
}

void Instr32::dec_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32-1);
}

void Instr32::call_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();

	PUSH32(GET_EIP());
	SET_EIP(rm32);
}

void Instr32::jmp_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	SET_EIP(rm32);
}

void Instr32::push_rm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	PUSH32(rm32);
}

