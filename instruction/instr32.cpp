#include <stdint.h>
#include "instruction/base.hpp"

#define instr32(f) ((instrfunc_t)&Instr32::f)

Instr32::Instr32(Emulator *e) : Instruction(e, true) {
	int i;

	// 0x00 : add_rm8_r8
	set_funcflag(0x01, instr32(add_rm32_r32), CHK_MODRM);
	// 0x02 : add_r8_rm8
	set_funcflag(0x03, instr32(add_r32_rm32), CHK_MODRM);
	// 0x04 : add_al_imm8
	set_funcflag(0x05, instr32(add_eax_imm32), CHK_IMM32);

	// 0x08 : or_rm8_r8
	set_funcflag(0x09, instr32(or_rm32_r32), CHK_MODRM);
	// 0x0a : or_r8_rm8
	set_funcflag(0x0b, instr32(or_r32_rm32), CHK_MODRM);
	// 0x0c : or_al_imm8
	set_funcflag(0x0d, instr32(or_eax_imm32), CHK_IMM32);

	// 0x20 : and_rm8_r8
	set_funcflag(0x21, instr32(and_rm32_r32), CHK_MODRM);
	// 0x22 : and_r8_rm8
	set_funcflag(0x23, instr32(and_r32_rm32), CHK_MODRM);
	// 0x24 : and_al_imm8
	set_funcflag(0x25, instr32(and_eax_imm32), CHK_IMM32);

	// 0x28 : sub_rm8_r8
	set_funcflag(0x29, instr32(sub_rm32_r32), CHK_MODRM);
	// 0x2a : sub_r8_rm8
	set_funcflag(0x2b, instr32(sub_r32_rm32), CHK_MODRM);
	// 0x2c : sub_al_imm8
	set_funcflag(0x2d, instr32(sub_eax_imm32), CHK_IMM32);

	// 0x30 : xor_rm8_r8
	set_funcflag(0x31, instr32(xor_rm32_r32), CHK_MODRM);
	// 0x32 : xor_r8_rm8
	set_funcflag(0x33, instr32(xor_r32_rm32), CHK_MODRM);
	// 0x34 : xor_al_imm8
	set_funcflag(0x35, instr32(xor_eax_imm32), CHK_IMM32);

	// 0x38 : cmp_rm8_r8
	set_funcflag(0x39, instr32(cmp_rm32_r32), CHK_MODRM);
	// 0x3a : cmp_r8_rm8
	set_funcflag(0x3b, instr32(cmp_r32_rm32), CHK_MODRM);
	// 0x3c : cmp_al_imm8
	set_funcflag(0x3d, instr32(cmp_eax_imm32), CHK_IMM32);

	for (i=0; i<8; i++)	set_funcflag(0x40+i, instr32(inc_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x48+i, instr32(dec_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x50+i, instr32(push_r32) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x58+i, instr32(pop_r32) ,0);

	set_funcflag(0x60, instr32(pushad), 0);
	set_funcflag(0x61, instr32(popad), 0);

	set_funcflag(0x68, instr32(push_imm32), CHK_IMM32);
	//set_funcflag(0x69, instr32(imul_r32_rm32_imm32), CHK_MODRM|CHK_IMM32);
	set_funcflag(0x6a, instr32(push_imm8), CHK_IMM8);
	//set_funcflag(0x6b, instr32(imul_r32_rm32_imm8), CHK_MODRM|CHK_IMM8);

	// 0x70-0x7f : jcc

	// 0x84 : test_rm8_r8
	set_funcflag(0x85, instr32(test_rm32_r32), CHK_MODRM);
	// 0x86 : xchg_r8_rm8
	set_funcflag(0x87, instr32(xchg_r32_rm32), CHK_MODRM);
	// 0x88 : mov_rm8_r8
	set_funcflag(0x89, instr32(mov_rm32_r32), CHK_MODRM);
	// 0x8a : mov_r8_rm8
	set_funcflag(0x8b, instr32(mov_r32_rm32), CHK_MODRM);
	set_funcflag(0x8c, instr32(mov_rm32_sreg), CHK_MODRM);
	set_funcflag(0x8d, instr32(lea_r32_m32), CHK_MODRM);
	// 0x8e : mov_sreg_rm16

	// 0x90 : nop
	for (i=1; i<8; i++)	set_funcflag(0x90+i, instr32(xchg_r32_eax) ,CHK_IMM32);
	set_funcflag(0x99, instr32(cdq), 0);
	set_funcflag(0x9a, instr32(callf_ptr16_32), CHK_PTR16 | CHK_IMM32);

	// 0xb0-0xb7 : mov_r8_imm
	for (i=0; i<8; i++)	set_funcflag(0xb8+i, instr32(mov_r32_imm32) ,CHK_IMM32);

	set_funcflag(0xc3, instr32(ret), 0);

	set_funcflag(0xc7, instr32(mov_rm32_imm32), CHK_MODRM | CHK_IMM32);

	set_funcflag(0xc9, instr32(leave), 0);

	// 0xcd : int_imm8

	// 0xcf : iret

	// 0xe4 : in_al_imm8
	set_funcflag(0xe5, instr32(in_eax_imm8), CHK_IMM8);
	// 0xe6 : out_imm8_al
	set_funcflag(0xe7, instr32(out_imm8_eax), CHK_IMM8);
	set_funcflag(0xe8, instr32(call_rel32), CHK_IMM32);
	set_funcflag(0xe9, instr32(jmp_rel32), CHK_IMM32);
	set_funcflag(0xea, instr32(jmpf_ptr16_32), CHK_PTR16 | CHK_IMM32);
	// 0xeb : jmp
	// 0xec : in_al_dx
	set_funcflag(0xed, instr32(in_eax_dx), 0);
	// 0xee : out_dx_al
	set_funcflag(0xef, instr32(out_dx_eax), 0);

	set_funcflag(0x0faf, instr32(imul_r32_rm32), CHK_MODRM);
	set_funcflag(0x0fb6, instr32(movzx_r32_rm8), CHK_MODRM);
	set_funcflag(0x0fb7, instr32(movzx_r32_rm16), CHK_MODRM);
	set_funcflag(0x0fbe, instr32(movsx_r32_rm8), CHK_MODRM);
	set_funcflag(0x0fbf, instr32(movsx_r32_rm16), CHK_MODRM);

	// 0x80 : code_80
	set_funcflag(0x81, instr32(code_81), CHK_MODRM | CHK_IMM32);
	// 0x82 : code_82
	set_funcflag(0x83, instr32(code_83), CHK_MODRM | CHK_IMM8);
	set_funcflag(0xc1, instr32(code_c1), CHK_MODRM | CHK_IMM8);
	set_funcflag(0xff, instr32(code_ff), CHK_MODRM);
	set_funcflag(0x0f00, instr32(code_0f00), CHK_MODRM);
	set_funcflag(0x0f01, instr32(code_0f01), CHK_MODRM);
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

void Instr32::add_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EAX, eax+IMM32);
	EFLAGS_UPDATE_ADD(eax, IMM32);
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

void Instr32::or_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EAX, eax|IMM32);
	EFLAGS_UPDATE_OR(eax, IMM32);
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
	set_r32(r32&rm32);
	EFLAGS_UPDATE_AND(r32, rm32);
}

void Instr32::and_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EAX, eax&IMM32);
	EFLAGS_UPDATE_AND(eax, IMM32);
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

void Instr32::sub_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EAX, eax-IMM32);
	EFLAGS_UPDATE_SUB(eax, IMM32);
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

void Instr32::xor_eax_imm32(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EAX, eax^IMM32);
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

void Instr32::pushad(void){
	uint32_t esp;
	
	esp = GET_GPREG(ESP);

	PUSH32(GET_GPREG(EAX));
	PUSH32(GET_GPREG(ECX));
	PUSH32(GET_GPREG(EDX));
	PUSH32(GET_GPREG(EBX));
	PUSH32(esp);
	PUSH32(GET_GPREG(EBP));
	PUSH32(GET_GPREG(ESI));
	PUSH32(GET_GPREG(EDI));
}

void Instr32::popad(void){
	uint32_t esp;

	SET_GPREG(EDI, POP32());
	SET_GPREG(ESI, POP32());
	SET_GPREG(EBP, POP32());
	esp = POP32();
	SET_GPREG(EBX, POP32());
	SET_GPREG(EDX, POP32());
	SET_GPREG(ECX, POP32());
	SET_GPREG(EAX, POP32());

	SET_GPREG(ESP, esp);
}

void Instr32::push_imm32(void){
	PUSH32(IMM32);
}

void Instr32::push_imm8(void){
	PUSH32(IMM8);
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

void Instr32::lea_r32_m32(void){
	uint32_t m32;

	m32 = get_m();
	set_r32(m32);
}

void Instr32::xchg_r32_eax(void){
	uint32_t r32, eax;

	r32 = get_r32();
	eax = GET_GPREG(EAX);
	set_r32(eax);
	SET_GPREG(EAX, r32);
}

void Instr32::cdq(void){
	uint32_t eax;

	eax = GET_GPREG(EAX);
	SET_GPREG(EDX, eax&(1<<31) ? -1 : 0);
}

void Instr32::callf_ptr16_32(void){
	EMU->set_sgreg(CS, PTR16);
	PUSH32(GET_EIP());
	SET_EIP(IMM32);
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

void Instr32::jmpf_ptr16_32(void){
	EMU->set_sgreg(CS, PTR16);
	SET_EIP(IMM32);
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

void Instr32::imul_r32_rm32(void){
	uint16_t r32, rm32;

	r32 = get_r32();
	rm32 = get_rm32();
	set_r32(r32*rm32);
	EFLAGS_UPDATE_MUL(r32, rm32);
}

void Instr32::movzx_r32_rm8(void){
	uint8_t rm8;

	rm8 = get_rm8();
	set_r32(rm8);
}

void Instr32::movzx_r32_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_r32(rm16);
}

void Instr32::movsx_r32_rm8(void){
	int8_t rm8_s;

	rm8_s = get_rm8();
	set_r32(rm8_s);
}

void Instr32::movsx_r32_rm16(void){
	int16_t rm16_s;

	rm16_s = get_rm16();
	set_r32(rm16_s);
}

/******************************************************************/

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

void Instr32::code_c1(void){
	switch(REG){
		case 4: shl_rm32_imm8();        break;
		case 5: shr_rm32_imm8();        break;
		//case 7: sar_rm32_imm8();        break;
		default:
			ERROR("not implemented: 0xc1 /%d\n", REG);
	}
}

void Instr32::code_ff(void){
	switch(REG){
		case 0:	inc_rm32();		break;
		case 1:	dec_rm32();		break;
		case 2:	call_rm32();		break;
		case 4:	jmp_rm32();		break;
		case 6:	push_rm32();		break;
		default:
			ERROR("not implemented: 0xff /%d\n", REG);
	}
}

void Instr32::code_0f00(void){
	switch(REG){
		case 3: ltr_rm16();		break;
		default:
			ERROR("not implemented: 0x0f00 /%d\n", REG);
	}
}

void Instr32::code_0f01(void){
	switch(REG){
		case 2: lgdt_m32();		break;
		case 3: lidt_m32();		break;
		default:
			ERROR("not implemented: 0x0f01 /%d\n", REG);
	}
}

/******************************************************************/

void Instr32::add_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32+IMM32);
	EFLAGS_UPDATE_ADD(rm32, IMM32);
}

void Instr32::or_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32|IMM32);
	EFLAGS_UPDATE_OR(rm32, IMM32);
}

void Instr32::adc_rm32_imm32(void){
	uint32_t rm32;
	uint8_t cf;

	rm32 = get_rm32();
	cf = EFLAGS_CF;
	set_rm32(rm32+IMM32+cf);
	EFLAGS_UPDATE_ADD(rm32, IMM32+cf);
}

void Instr32::sbb_rm32_imm32(void){
	uint32_t rm32;
	uint8_t cf;

	rm32 = get_rm32();
	cf = EFLAGS_CF;
	set_rm32(rm32-IMM32-cf);
	EFLAGS_UPDATE_SUB(rm32, IMM32+cf);
}

void Instr32::and_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32&IMM32);
	EFLAGS_UPDATE_AND(rm32, IMM32);
}

void Instr32::sub_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32-IMM32);
	EFLAGS_UPDATE_SUB(rm32, IMM32);
}

void Instr32::xor_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32^IMM32);
}

void Instr32::cmp_rm32_imm32(void){
	uint32_t rm32;

	rm32 = get_rm32();
	EFLAGS_UPDATE_SUB(rm32, IMM32);
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

void Instr32::shl_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32<<IMM8);
	EFLAGS_UPDATE_SHL(rm32, IMM8);
}

void Instr32::shr_rm32_imm8(void){
	uint32_t rm32;

	rm32 = get_rm32();
	set_rm32(rm32>>IMM8);
	EFLAGS_UPDATE_SHR(rm32, IMM8);
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

/******************************************************************/

void Instr32::lgdt_m32(void){
	uint32_t m32;

	m32 = get_m();
	INFO("m32 = 0x%08x", m32);
	EMU->set_dtreg(GDTR, READ_MEM32(m32+2), READ_MEM16(m32));
}

void Instr32::lidt_m32(void){
	uint32_t m32;

	m32 = get_m();
	INFO("m32 = 0x%08x", m32);
	EMU->set_dtreg(IDTR, READ_MEM32(m32+2), READ_MEM16(m32));
}

