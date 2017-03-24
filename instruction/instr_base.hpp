#ifndef _INSTR_BASE_H
#define _INSTR_BASE_H

#include <string.h>
#include "../common.hpp"
#include "instruction.hpp"

#define GET_GPREG(reg) 			emu->get_gpreg(reg)
#define SET_GPREG(reg, v) 		emu->set_gpreg(reg, v)
#define UPDATE_GPREG(reg, v) 		emu->update_gpreg(reg, v)
#define EFLAGS_UPDATE_ADD(v1, v2)	emu->p_eflags()->update_add(v1, v2)
#define EFLAGS_UPDATE_OR(v1, v2)	emu->p_eflags()->update_or(v1, v2)
#define EFLAGS_UPDATE_AND(v1, v2)	emu->p_eflags()->update_and(v1, v2)
#define EFLAGS_UPDATE_SUB(v1, v2)	emu->p_eflags()->update_sub(v1, v2)
#define EFLAGS_CF			(emu->p_eflags()->is_carry())
#define EFLAGS_PF			(emu->p_eflags()->is_parity())
#define EFLAGS_ZF			(emu->p_eflags()->is_zero())
#define EFLAGS_SF			(emu->p_eflags()->is_sign())
#define EFLAGS_OF			(emu->p_eflags()->is_overflow())

class InstrBase : public ExecInstr , public ParseInstr {
	public:
		InstrBase() { memset(instrfuncs, 0, sizeof(instrfuncs)); memset(chk, 0, sizeof(chk)); };
	protected:
		void set_funcflag(uint8_t idx, instrfunc_t func, uint8_t flags){ instrfuncs[idx] = func; chk[idx].flags = flags; };
};

class Instr16 : public InstrBase {
        public:
                Instr16(Emulator *e) { emu = e; init_instr(); };
        private:
                void init_instr(void);
};

class Instr32 : public InstrBase {
        public:
                Instr32(Emulator *e) { emu = e; init_instr(); };
        private:
                void init_instr(void);

		void add_rm8_r8(void);
		void add_rm32_r32(void);
		void add_r8_rm8(void);
		void add_r32_rm32(void);
		void or_rm8_r8(void);
		void or_rm32_r32(void);
		void or_r8_rm8(void);
		void or_r32_rm32(void);
		void and_rm8_r8(void);
		void and_rm32_r32(void);
		void and_r8_rm8(void);
		void and_r32_rm32(void);
		void sub_rm8_r8(void);
		void sub_rm32_r32(void);
		void sub_r8_rm8(void);
		void sub_r32_rm32(void);
		void xor_rm8_r8(void);
		void xor_rm32_r32(void);
		void xor_r8_rm8(void);
		void xor_r32_rm32(void);
		void cmp_rm8_r8(void);
		void cmp_rm32_r32(void);
		void cmp_r8_rm8(void);
		void cmp_r32_rm32(void);
		void cmp_al_imm8(void);
		void cmp_eax_imm32(void);
		void inc_r32(void);
		void dec_r32(void);
		void push_r32(void);
		void pop_r32(void);
		void push_imm32(void);
		void push_imm8(void);
		void jo(void);
		void jno(void);
		void jb(void);
		void jnb(void);
		void jz(void);
		void jnz(void);
		void jbe(void);
		void ja(void);
		void js(void);
		void jns(void);
		void jp(void);
		void jnp(void);
		void jl(void);
		void jnl(void);
		void jle(void);
		void jnle(void);
		//void code_80(void);
		//void code_81(void);
		//void code_82(void);
		void code_83(void);
		void test_rm8_r8(void);
		void test_rm32_r32(void);
		void xchg_r8_rm8(void);
		void xchg_r32_rm32(void);
		void mov_rm8_r8(void);
		void mov_rm32_r32(void);
		void mov_r8_rm8(void);
		void mov_r32_rm32(void);
		void nop(void);
		void xchg_r32_eax(void);
                void mov_r8_imm8(void);
                void mov_r32_imm32(void);
                void ret(void);
                void mov_rm32_imm32(void);
                void leave(void);
		void in_al_imm8(void);
		void in_eax_imm8(void);
		void out_imm8_al(void);
		void out_imm8_eax(void);
                void call_rel32(void);
                void jmp_rel32(void);
                void jmp_rel8(void);
		void in_al_dx(void);
		void in_eax_dx(void);
		void out_dx_al(void);
		void out_dx_eax(void);
                void code_ff(void);

		// 0x83
		void add_rm32_imm8(void);
		void or_rm32_imm8(void);
		void adc_rm32_imm8(void);
		void sbb_rm32_imm8(void);
		void and_rm32_imm8(void);
		void sub_rm32_imm8(void);
		void xor_rm32_imm8(void);
		void cmp_rm32_imm8(void);

		//0xff
		void inc_rm32(void);
		void dec_rm32(void);
		void call_rm32(void);
		void jmp_rm32(void);
		void push_rm32(void);
};

#endif
