#include <stdint.h>
#include "instruction/base.hpp"

void InstrBase::add_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	set_rm8(rm8+r8);
	EFLAGS_UPDATE_ADD(rm8, r8);
}

void InstrBase::add_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(r8+rm8);
	EFLAGS_UPDATE_ADD(r8, rm8);
}

void InstrBase::or_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	set_rm8(rm8|r8);
	EFLAGS_UPDATE_OR(rm8, r8);
}

void InstrBase::or_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(r8|rm8);
	EFLAGS_UPDATE_OR(r8, rm8);
}

void InstrBase::and_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	set_rm8(rm8&r8);
	EFLAGS_UPDATE_AND(rm8, r8);
}

void InstrBase::and_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(r8|rm8);
	EFLAGS_UPDATE_AND(r8, rm8);
}

void InstrBase::sub_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	set_rm8(rm8-r8);
	EFLAGS_UPDATE_SUB(rm8, r8);
}

void InstrBase::sub_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(r8-rm8);
	EFLAGS_UPDATE_SUB(r8, rm8);
}

void InstrBase::xor_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	set_rm8(rm8^r8);
}

void InstrBase::xor_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(r8^rm8);
}

void InstrBase::cmp_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	EFLAGS_UPDATE_SUB(rm8, r8);
}

void InstrBase::cmp_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	EFLAGS_UPDATE_SUB(r8, rm8);
}

void InstrBase::cmp_al_imm8(void){
	uint8_t al;

	al = GET_GPREG(AL);
	EFLAGS_UPDATE_SUB(al, IMM8);
}

#define JCC_REL8(cc, is_flag) \
void InstrBase::j ## cc(void){ \
	if(is_flag) \
		UPDATE_EIP(IMM8); \
}

JCC_REL8(o, EFLAGS_OF)
JCC_REL8(no, !EFLAGS_OF)
JCC_REL8(b, EFLAGS_CF)
JCC_REL8(nb, !EFLAGS_CF)
JCC_REL8(z, EFLAGS_ZF)
JCC_REL8(nz, !EFLAGS_ZF)
JCC_REL8(be, EFLAGS_CF || EFLAGS_ZF)
JCC_REL8(a, !(EFLAGS_CF || EFLAGS_ZF))
JCC_REL8(s, EFLAGS_SF)
JCC_REL8(ns, !EFLAGS_SF)
JCC_REL8(p, EFLAGS_PF)
JCC_REL8(np, !EFLAGS_PF)
JCC_REL8(l, EFLAGS_SF != EFLAGS_OF)
JCC_REL8(nl, EFLAGS_SF == EFLAGS_OF)
JCC_REL8(le, EFLAGS_ZF || (EFLAGS_SF != EFLAGS_OF))
JCC_REL8(nle, !EFLAGS_ZF && (EFLAGS_SF == EFLAGS_OF))

void InstrBase::test_rm8_r8(void){
	uint8_t rm8, r8;

	rm8 = get_rm8();
	r8 = get_r8();
	EFLAGS_UPDATE_AND(rm8, r8);
}

void InstrBase::xchg_r8_rm8(void){
	uint8_t r8, rm8;

	r8 = get_r8();
	rm8 = get_rm8();
	set_r8(rm8);
	set_rm8(r8);
}

void InstrBase::mov_rm8_r8(void){
	uint8_t r8;

	r8 = get_r8();
	set_rm8(r8);
}

void InstrBase::mov_r8_rm8(void){
	uint8_t rm8;

	rm8 = get_rm8();
	set_r8(rm8);
}

void InstrBase::mov_sreg_rm16(void){
	uint16_t rm16;

	rm16 = get_rm16();
	set_sreg(rm16);
}

void InstrBase::nop(void){} 		// xchg eax, eax

void InstrBase::mov_r8_imm8(void){
	uint8_t reg;

	reg = OPCODE & ((1<<3)-1);
	SET_GPREG(static_cast<reg8_t>(reg), IMM8);
}

void InstrBase::int_imm8(void){
	EMU->interrupt_hundle(IMM8);
}

void InstrBase::iret(void){
	EMU->iret();
}

void InstrBase::in_al_imm8(void){
	SET_GPREG(AL, EMU->in_io8(IMM8));
}

void InstrBase::out_imm8_al(void){
	uint8_t al;

	al = GET_GPREG(AL);
	EMU->out_io8(IMM8, al);
}

void InstrBase::jmp_rel8(void){
	UPDATE_EIP(IMM8);
}

void InstrBase::in_al_dx(void){
	uint16_t dx;

	dx = GET_GPREG(DX);
	SET_GPREG(AL, EMU->in_io8(dx));
}

void InstrBase::out_dx_al(void){
	uint16_t dx;
	uint8_t al;

	dx = GET_GPREG(DX);
	al = GET_GPREG(AL);
	EMU->out_io8(dx, al);
}
