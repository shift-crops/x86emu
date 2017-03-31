#include <stdint.h>
#include "instruction/base.hpp"

#define instr16(f) ((instrfunc_t)&Instr16::f)

Instr16::Instr16(Emulator *e) : Instruction(e, false) {
	int i;

	// 0x00 : add_rm8_r8
	set_funcflag(0x01, instr16(add_rm16_r16), CHK_MODRM);
	// 0x02 : add_r8_rm8
	set_funcflag(0x03, instr16(add_r16_rm16), CHK_MODRM);
	// 0x04 : add_al_imm8
	set_funcflag(0x05, instr16(add_ax_imm16), CHK_IMM16);

	// 0x08 : or_rm8_r8
	set_funcflag(0x09, instr16(or_rm16_r16), CHK_MODRM);
	// 0x0a : or_r8_rm8
	set_funcflag(0x0b, instr16(or_r16_rm16), CHK_MODRM);
	// 0x0c : or_al_imm8
	set_funcflag(0x0d, instr16(or_ax_imm16), CHK_IMM16);

	// 0x20 : and_rm8_r8
	set_funcflag(0x21, instr16(and_rm16_r16), CHK_MODRM);
	// 0x22 : and_r8_rm8
	set_funcflag(0x23, instr16(and_r16_rm16), CHK_MODRM);
	// 0x24 : and_al_imm8
	set_funcflag(0x25, instr16(and_ax_imm16), CHK_IMM16);

	// 0x28 : sub_rm8_r8
	set_funcflag(0x29, instr16(sub_rm16_r16), CHK_MODRM);
	// 0x2a : sub_r8_rm8
	set_funcflag(0x2b, instr16(sub_r16_rm16), CHK_MODRM);
	// 0x2c : sub_al_imm8
	set_funcflag(0x2d, instr16(sub_ax_imm16), CHK_IMM16);

	// 0x30 : xor_rm8_r8
	set_funcflag(0x31, instr16(xor_rm16_r16), CHK_MODRM);
	// 0x16 : xor_r8_rm8
	set_funcflag(0x33, instr16(xor_r16_rm16), CHK_MODRM);
	// 0x34 : xor_al_imm8
	set_funcflag(0x35, instr16(xor_ax_imm16), CHK_IMM16);

	// 0x38 : cmp_rm8_r8
	set_funcflag(0x39, instr16(cmp_rm16_r16), CHK_MODRM);
	// 0x3a : cmp_r8_rm8
	set_funcflag(0x3b, instr16(cmp_r16_rm16), CHK_MODRM);
	// 0x3c : cmp_al_imm8
	set_funcflag(0x3d, instr16(cmp_ax_imm16), CHK_IMM16);

	for (i=0; i<8; i++)	set_funcflag(0x40+i, instr16(inc_r16) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x48+i, instr16(dec_r16) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x50+i, instr16(push_r16) ,0);
	for (i=0; i<8; i++)	set_funcflag(0x58+i, instr16(pop_r16) ,0);

	set_funcflag(0x68, instr16(push_imm16), CHK_IMM16);
	//set_funcflag(0x69, instr16(imul_r16_rm16_imm16), CHK_MODRM|CHK_IMM16);
	set_funcflag(0x6a, instr16(push_imm8), CHK_IMM8);
	//set_funcflag(0x6b, instr16(imul_r16_rm16_imm8), CHK_MODRM|CHK_IMM8);

	// 0x70-0x7f : jcc

	// 0x84 : test_rm8_r8
	set_funcflag(0x85, instr16(test_rm16_r16), CHK_MODRM);
	// 0x86 : xchg_r8_rm8
	set_funcflag(0x87, instr16(xchg_r16_rm16), CHK_MODRM);
	// 0x88 : mov_rm8_r8
	set_funcflag(0x89, instr16(mov_rm16_r16), CHK_MODRM);
	// 0x8a : mov_r8_rm8
	set_funcflag(0x8b, instr16(mov_r16_rm16), CHK_MODRM);
	set_funcflag(0x8c, instr16(mov_rm16_sreg), CHK_MODRM);
	set_funcflag(0x8d, instr16(lea_r16_m), CHK_MODRM);
	// 0x8e : mov_sreg_rm16

	// 0x90 : nop
	for (i=1; i<8; i++)	set_funcflag(0x90+i, instr16(xchg_r16_ax) ,CHK_IMM16);

	// 0xb0-0xb7 : mov_r8_imm
	for (i=0; i<8; i++)	set_funcflag(0xb8+i, instr16(mov_r16_imm16) ,CHK_IMM16);

	set_funcflag(0xc3, instr16(ret), 0);

	set_funcflag(0xc7, instr16(mov_rm16_imm16), CHK_MODRM | CHK_IMM16);

	set_funcflag(0xc9, instr16(leave), 0);

	// 0xcd : int_imm8

	// 0xcf : iret

	// 0xe4 : in_al_imm8
	set_funcflag(0xe5, instr16(in_ax_imm8), CHK_IMM8);
	// 0xe6 : out_imm8_al
	set_funcflag(0xe7, instr16(out_imm8_ax), CHK_IMM8);
	set_funcflag(0xe8, instr16(call_rel16), CHK_IMM16);
	set_funcflag(0xe9, instr16(jmp_rel16), CHK_IMM16);
	set_funcflag(0xea, instr16(jmp_ptr16_16), CHK_PTR16 | CHK_IMM16);
	// 0xeb : jmp
	// 0xec : in_al_dx
	set_funcflag(0xed, instr16(in_ax_dx), 0);
	// 0xee : out_dx_al
	set_funcflag(0xef, instr16(out_dx_ax), 0);

	set_funcflag(0x0faf, instr16(imul_r16_rm16), CHK_MODRM);
	set_funcflag(0x0fb6, instr16(movzx_r16_rm8), CHK_MODRM);
	set_funcflag(0x0fb7, instr16(movzx_r16_rm16), CHK_MODRM);
	set_funcflag(0x0fbe, instr16(movsx_r16_rm8), CHK_MODRM);
	set_funcflag(0x0fbf, instr16(movsx_r16_rm16), CHK_MODRM);

	// 0x80 : code_80
	set_funcflag(0x81, instr16(code_81), CHK_MODRM | CHK_IMM16);
	// 0x82 : code_82
	set_funcflag(0x83, instr16(code_83), CHK_MODRM | CHK_IMM8);
	set_funcflag(0xc1, instr16(code_c1), CHK_MODRM | CHK_IMM8);
	set_funcflag(0xff, instr16(code_ff), CHK_MODRM);
	set_funcflag(0x0f00, instr16(code_0f00), CHK_MODRM);
	set_funcflag(0x0f01, instr16(code_0f01), CHK_MODRM);
}

void Instr16::add_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	set_rm16(rm16+r16);
	EFLAGS_UPDATE_ADD(rm16, r16);
}

void Instr16::add_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16+rm16);
	EFLAGS_UPDATE_ADD(r16, rm16);
}

void Instr16::add_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	SET_GPREG(AX, ax+IMM16);
	EFLAGS_UPDATE_ADD(ax, IMM16);
}

void Instr16::or_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	set_rm16(rm16|r16);
	EFLAGS_UPDATE_OR(rm16, r16);
}

void Instr16::or_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16|rm16);
	EFLAGS_UPDATE_OR(r16, rm16);
}

void Instr16::or_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	SET_GPREG(AX, ax|IMM16);
	EFLAGS_UPDATE_OR(ax, IMM16);
}

void Instr16::and_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	set_rm16(rm16&r16);
	EFLAGS_UPDATE_AND(rm16, r16);
}

void Instr16::and_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16&rm16);
	EFLAGS_UPDATE_AND(r16, rm16);
}

void Instr16::and_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	SET_GPREG(AX, ax&IMM16);
	EFLAGS_UPDATE_AND(ax, IMM16);
}

void Instr16::sub_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	set_rm16(rm16-r16);
	EFLAGS_UPDATE_SUB(rm16, r16);
}

void Instr16::sub_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16-rm16);
	EFLAGS_UPDATE_SUB(r16, rm16);
}

void Instr16::sub_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	SET_GPREG(AX, ax-IMM16);
	EFLAGS_UPDATE_SUB(ax, IMM16);
}

void Instr16::xor_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	set_rm16(rm16^r16);
}

void Instr16::xor_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16^rm16);
}

void Instr16::xor_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	SET_GPREG(AX, ax^IMM16);
}

void Instr16::cmp_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	EFLAGS_UPDATE_SUB(rm16, r16);
}

void Instr16::cmp_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	EFLAGS_UPDATE_SUB(r16, rm16);
}

void Instr16::cmp_ax_imm16(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	EFLAGS_UPDATE_SUB(ax, IMM16);
}

void Instr16::inc_r16(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	UPDATE_GPREG(static_cast<reg16_t>(reg), 1);
}

void Instr16::dec_r16(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	UPDATE_GPREG(static_cast<reg16_t>(reg), -1);
}

void Instr16::push_r16(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	PUSH16(GET_GPREG(static_cast<reg16_t>(reg)));
}

void Instr16::pop_r16(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	SET_GPREG(static_cast<reg16_t>(reg), POP16());
}

void Instr16::push_imm16(void){
	PUSH16(IMM16);
}

void Instr16::push_imm8(void){
	PUSH16(IMM8);
}

void Instr16::test_rm16_r16(void){
	uint16_t rm16, r16;

	rm16 = get_rm16();
	r16 = get_r16();
	EFLAGS_UPDATE_AND(rm16, r16);
}

void Instr16::xchg_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(rm16);
	set_rm16(r16);
}

void Instr16::mov_rm16_r16(void){
	uint16_t r16;

	r16 = get_r16();
	set_rm16(r16);
}

void Instr16::mov_r16_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_r16(rm16);
}

void Instr16::mov_rm16_sreg(void){
	uint16_t sreg;

	sreg = get_sreg();
	set_rm16(sreg);
}

void Instr16::lea_r16_m(void){
	set_r16(get_m());
}

void Instr16::xchg_r16_ax(void){
	uint16_t r16, ax;

	r16 = get_r16();
	ax = GET_GPREG(AX);
	set_r16(ax);
	SET_GPREG(AX, r16);
}

void Instr16::mov_r16_imm16(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	SET_GPREG(static_cast<reg16_t>(reg), IMM16);
}

void Instr16::ret(void){
	uint16_t addr;

	addr = POP16();
	SET_IP(addr);
}

void Instr16::mov_rm16_imm16(void){
	set_rm16(IMM16);
}

void Instr16::leave(void){
	uint16_t ebp;

	ebp = GET_GPREG(EBP);
	SET_GPREG(ESP, ebp);
	SET_GPREG(EBP, POP16());
}

void Instr16::in_ax_imm8(void){
	SET_GPREG(AX, EMU->in_io16(IMM8));
}

void Instr16::out_imm8_ax(void){
	uint16_t ax;

	ax = GET_GPREG(AX);
	EMU->out_io16(IMM8, ax);
}

void Instr16::call_rel16(void){
	PUSH16(GET_IP());
	UPDATE_IP(IMM16);
}

void Instr16::jmp_rel16(void){
	UPDATE_IP(IMM16);
}

void Instr16::jmp_ptr16_16(void){
	EMU->set_sgreg(CS, PTR16);
	SET_IP(IMM16);
}

void Instr16::in_ax_dx(void){
	uint16_t dx;

	dx = GET_GPREG(DX);
	SET_GPREG(AX, EMU->in_io16(dx));
}

void Instr16::out_dx_ax(void){
	uint16_t dx, ax;

	dx = GET_GPREG(DX);
	ax = GET_GPREG(AX);
	EMU->out_io16(dx, ax);
}

void Instr16::imul_r16_rm16(void){
	uint16_t r16, rm16;

	r16 = get_r16();
	rm16 = get_rm16();
	set_r16(r16*rm16);
	EFLAGS_UPDATE_MUL(r16, rm16);
}

void Instr16::movzx_r16_rm8(void){
	uint8_t rm8;

	rm8 = get_rm8();
	set_r16(rm8);
}

void Instr16::movzx_r16_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_r16(rm16);
}

void Instr16::movsx_r16_rm8(void){
	int8_t rm8_s;

	rm8_s = get_rm8();
	set_r16(rm8_s);
}

void Instr16::movsx_r16_rm16(void){
	int16_t rm16_s;

	rm16_s = get_rm16();
	set_r16(rm16_s);
}

/******************************************************************/

void Instr16::code_81(void){
	switch(REG){
		case 0:	add_rm16_imm16();	break;
		case 1:	or_rm16_imm16();	break;
		case 2:	adc_rm16_imm16();	break;
		case 3:	sbb_rm16_imm16();	break;
		case 4:	and_rm16_imm16();	break;
		case 5:	sub_rm16_imm16();	break;
		case 6:	xor_rm16_imm16();	break;
		case 7:	cmp_rm16_imm16();	break;
		default:
			ERROR("not implemented: 0x81 /%d\n", REG);
	}
}

void Instr16::code_83(void){
	switch(REG){
		case 0:	add_rm16_imm8();	break;
		case 1:	or_rm16_imm8();		break;
		case 2:	adc_rm16_imm8();	break;
		case 3:	sbb_rm16_imm8();	break;
		case 4:	and_rm16_imm8();	break;
		case 5:	sub_rm16_imm8();	break;
		case 6:	xor_rm16_imm8();	break;
		case 7:	cmp_rm16_imm8();	break;
		default:
			ERROR("not implemented: 0x83 /%d\n", REG);
	}
}

void Instr16::code_c1(void){
	switch(REG){
		case 4: shl_rm16_imm8();        break;
		case 5: shr_rm16_imm8();        break;
		//case 7: sar_rm16_imm8();        break;
		default:
			ERROR("not implemented: 0xc1 /%d\n", REG);
	}
}

void Instr16::code_ff(void){
	switch(REG){
		case 0:	inc_rm16();		break;
		case 1:	dec_rm16();		break;
		case 2:	call_rm16();		break;
		case 4:	jmp_rm16();		break;
		case 6:	push_rm16();		break;
		default:
			ERROR("not implemented: 0xff /%d\n", REG);
	}
}

void Instr16::code_0f00(void){
	switch(REG){
		case 3: ltr_rm16();		break;
		default:
			ERROR("not implemented: 0x0f00 /%d\n", REG);
	}
}

void Instr16::code_0f01(void){
	switch(REG){
		case 2: lgdt_m16();		break;
		case 3: lidt_m16();		break;
		default:
			ERROR("not implemented: 0x0f01 /%d\n", REG);
	}
}

/******************************************************************/

void Instr16::add_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16+IMM16);
	EFLAGS_UPDATE_ADD(rm16, IMM16);
}

void Instr16::or_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16|IMM16);
	EFLAGS_UPDATE_OR(rm16, IMM16);
}

void Instr16::adc_rm16_imm16(void){
	uint16_t rm16;
	uint8_t cf;

	rm16 = get_rm16();
	cf = EFLAGS_CF;
	set_rm16(rm16+IMM16+cf);
	EFLAGS_UPDATE_ADD(rm16, IMM16+cf);
}

void Instr16::sbb_rm16_imm16(void){
	uint16_t rm16;
	uint8_t cf;

	rm16 = get_rm16();
	cf = EFLAGS_CF;
	set_rm16(rm16-IMM16-cf);
	EFLAGS_UPDATE_SUB(rm16, IMM16+cf);
}

void Instr16::and_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16&IMM16);
	EFLAGS_UPDATE_AND(rm16, IMM16);
}

void Instr16::sub_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16-IMM16);
	EFLAGS_UPDATE_SUB(rm16, IMM16);
}

void Instr16::xor_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16^IMM16);
}

void Instr16::cmp_rm16_imm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	EFLAGS_UPDATE_SUB(rm16, IMM16);
}

/******************************************************************/

void Instr16::add_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16+IMM8);
	EFLAGS_UPDATE_ADD(rm16, IMM8);
}

void Instr16::or_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16|IMM8);
	EFLAGS_UPDATE_OR(rm16, IMM8);
}

void Instr16::adc_rm16_imm8(void){
	uint16_t rm16;
	uint8_t cf;

	rm16 = get_rm16();
	cf = EFLAGS_CF;
	set_rm16(rm16+IMM8+cf);
	EFLAGS_UPDATE_ADD(rm16, IMM8+cf);
}

void Instr16::sbb_rm16_imm8(void){
	uint16_t rm16;
	uint8_t cf;

	rm16 = get_rm16();
	cf = EFLAGS_CF;
	set_rm16(rm16-IMM8-cf);
	EFLAGS_UPDATE_SUB(rm16, IMM8+cf);
}

void Instr16::and_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16&IMM8);
	EFLAGS_UPDATE_AND(rm16, IMM8);
}

void Instr16::sub_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16-IMM8);
	EFLAGS_UPDATE_SUB(rm16, IMM8);
}

void Instr16::xor_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16^IMM8);
}

void Instr16::cmp_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	EFLAGS_UPDATE_SUB(rm16, IMM8);
}

/******************************************************************/

void Instr16::shl_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16<<IMM8);
	EFLAGS_UPDATE_SHL(rm16, IMM8);
}

void Instr16::shr_rm16_imm8(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16>>IMM8);
	EFLAGS_UPDATE_SHR(rm16, IMM8);
}

/******************************************************************/

void Instr16::inc_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16+1);
}

void Instr16::dec_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_rm16(rm16-1);
}

void Instr16::call_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();

	PUSH16(GET_IP());
	SET_IP(rm16);
}

void Instr16::jmp_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	SET_IP(rm16);
}

void Instr16::push_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	PUSH16(rm16);
}

/******************************************************************/

void Instr16::lgdt_m16(void){
	//INFO("DISP16:0x%04x data:0x%04x", DISP16, EMU->get_data32(SEGMENT, DISP16+2));
	SEGMENT = DS;
	EMU->set_dtreg(GDTR, READ_MEM32(DISP16+2)&((1<<24)-1), READ_MEM16(DISP16));
}

void Instr16::lidt_m16(void){
	SEGMENT = DS;
	EMU->set_dtreg(IDTR, READ_MEM32(DISP16+2)&((1<<24)-1), READ_MEM16(DISP16));
}

