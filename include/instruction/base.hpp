#ifndef _INSTR_BASE_H
#define _INSTR_BASE_H

#include <string.h>
#include "common.hpp"
#include "instruction.hpp"

class InstrBase : public ExecInstr , public ParseInstr {
	public:
		InstrBase();
		void set_chsz_ad(bool ad){ chsz_ad = ad; };
	protected:
		void set_funcflag(uint16_t opcode, instrfunc_t func, uint8_t flags){ instrfuncs[opcode] = func; chk[opcode].flags = flags; };

		void add_rm8_r8(void);
		void add_r8_rm8(void);
		void add_al_imm8(void);
		void or_rm8_r8(void);
		void or_r8_rm8(void);
		void or_al_imm8(void);
		void and_rm8_r8(void);
		void and_r8_rm8(void);
		void and_al_imm8(void);
		void sub_rm8_r8(void);
		void sub_r8_rm8(void);
		void sub_al_imm8(void);
		void xor_rm8_r8(void);
		void xor_r8_rm8(void);
		void xor_al_imm8(void);
		void cmp_rm8_r8(void);
		void cmp_r8_rm8(void);
		void cmp_al_imm8(void);
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
		void test_rm8_r8(void);
		void xchg_r8_rm8(void);
		void mov_rm8_r8(void);
		void mov_r8_rm8(void);
		void mov_sreg_rm16(void);
		void nop(void);
		void test_al_imm8(void);
                void mov_r8_imm8(void);
                void mov_rm8_imm8(void);
                void int_imm8(void);
                void iret(void);
		void in_al_imm8(void);
		void out_imm8_al(void);
                void jmp(void);
		void in_al_dx(void);
		void out_dx_al(void);
		void cli(void);
		void sti(void);
		void hlt(void);
		void ltr_rm16(void);
		void mov_r32_crn(void);
		void mov_crn_r32(void);
		void seto(void);
		void setno(void);
		void setb(void);
		void setnb(void);
		void setz(void);
		void setnz(void);
		void setbe(void);
		void seta(void);
		void sets(void);
		void setns(void);
		void setp(void);
		void setnp(void);
		void setl(void);
		void setnl(void);
		void setle(void);
		void setnle(void);

		void code_80(void);
		virtual void code_81(void) = 0;
		void code_82(void);
		virtual void code_83(void) = 0;
                virtual void code_ff(void) = 0;
                virtual void code_0f00(void) = 0;
                virtual void code_0f01(void) = 0;

		// 0x80, 0x82
		void add_rm8_imm8(void);
		void or_rm8_imm8(void);
		void adc_rm8_imm8(void);
		void sbb_rm8_imm8(void);
		void and_rm8_imm8(void);
		void sub_rm8_imm8(void);
		void xor_rm8_imm8(void);
		void cmp_rm8_imm8(void);

};

class Instr16 : public InstrBase {
        public:
                Instr16(Emulator *e);
        private:
		void add_rm16_r16(void);
		void add_r16_rm16(void);
		void add_ax_imm16(void);
		void or_rm16_r16(void);
		void or_r16_rm16(void);
		void or_ax_imm16(void);
		void and_rm16_r16(void);
		void and_r16_rm16(void);
		void and_ax_imm16(void);
		void sub_rm16_r16(void);
		void sub_r16_rm16(void);
		void sub_ax_imm16(void);
		void xor_rm16_r16(void);
		void xor_r16_rm16(void);
		void xor_ax_imm16(void);
		void cmp_rm16_r16(void);
		void cmp_r16_rm16(void);
		void cmp_ax_imm16(void);
		void inc_r16(void);
		void dec_r16(void);
		void push_r16(void);
		void pop_r16(void);
		void pusha(void);
		void popa(void);
		void push_imm16(void);
		void push_imm8(void);
		void test_rm16_r16(void);
		void xchg_r16_rm16(void);
		void mov_rm16_r16(void);
		void mov_r16_rm16(void);
		void mov_rm16_sreg(void);
		void lea_r16_m16(void);
		void xchg_r16_ax(void);
                void cwd(void);
                void callf_ptr16_16(void);
                void mov_r16_imm16(void);
                void ret(void);
                void leave(void);
                void mov_rm16_imm16(void);
		void in_ax_imm8(void);
		void out_imm8_ax(void);
                void call_rel16(void);
                void jmp_rel16(void);
                void jmpf_ptr16_16(void);
		void in_ax_dx(void);
		void out_dx_ax(void);
		void imul_r16_rm16(void);
		void movzx_r16_rm8(void);
		void movzx_r16_rm16(void);
		void movsx_r16_rm8(void);
		void movsx_r16_rm16(void);

		void code_81(void);
		void code_83(void);
                void code_c1(void);
                void code_ff(void);
                void code_0f00(void);
                void code_0f01(void);

		// 0x81
		void add_rm16_imm16(void);
		void or_rm16_imm16(void);
		void adc_rm16_imm16(void);
		void sbb_rm16_imm16(void);
		void and_rm16_imm16(void);
		void sub_rm16_imm16(void);
		void xor_rm16_imm16(void);
		void cmp_rm16_imm16(void);

		// 0x83
		void add_rm16_imm8(void);
		void or_rm16_imm8(void);
		void adc_rm16_imm8(void);
		void sbb_rm16_imm8(void);
		void and_rm16_imm8(void);
		void sub_rm16_imm8(void);
		void xor_rm16_imm8(void);
		void cmp_rm16_imm8(void);

		//0xc1
		void shl_rm16_imm8(void);
		void shr_rm16_imm8(void);

		//0xff
		void inc_rm16(void);
		void dec_rm16(void);
		void call_rm16(void);
		void jmp_rm16(void);
		void push_rm16(void);

		//0x0f01
		void lgdt_m16(void);
		void lidt_m16(void);
};

class Instr32 : public InstrBase {
        public:
                Instr32(Emulator *e);
        private:
		void add_rm32_r32(void);
		void add_r32_rm32(void);
		void add_eax_imm32(void);
		void or_rm32_r32(void);
		void or_r32_rm32(void);
		void or_eax_imm32(void);
		void and_rm32_r32(void);
		void and_r32_rm32(void);
		void and_eax_imm32(void);
		void sub_rm32_r32(void);
		void sub_r32_rm32(void);
		void sub_eax_imm32(void);
		void xor_rm32_r32(void);
		void xor_r32_rm32(void);
		void xor_eax_imm32(void);
		void cmp_rm32_r32(void);
		void cmp_r32_rm32(void);
		void cmp_eax_imm32(void);
		void inc_r32(void);
		void dec_r32(void);
		void push_r32(void);
		void pop_r32(void);
		void pushad(void);
		void popad(void);
		void push_imm32(void);
		void push_imm8(void);
		void test_rm32_r32(void);
		void xchg_r32_rm32(void);
		void mov_rm32_r32(void);
		void mov_r32_rm32(void);
		void mov_rm32_sreg(void);
		void lea_r32_m32(void);
		void xchg_r32_eax(void);
                void cdq(void);
                void callf_ptr16_32(void);
                void mov_r32_imm32(void);
                void ret(void);
                void leave(void);
                void mov_rm32_imm32(void);
		void in_eax_imm8(void);
		void out_imm8_eax(void);
                void call_rel32(void);
                void jmp_rel32(void);
                void jmpf_ptr16_32(void);
		void in_eax_dx(void);
		void out_dx_eax(void);
		void imul_r32_rm32(void);
		void movzx_r32_rm8(void);
		void movzx_r32_rm16(void);
		void movsx_r32_rm8(void);
		void movsx_r32_rm16(void);

		void code_81(void);
		void code_83(void);
		void code_c1(void);
                void code_ff(void);
                void code_0f00(void);
                void code_0f01(void);

		// 0x81
		void add_rm32_imm32(void);
		void or_rm32_imm32(void);
		void adc_rm32_imm32(void);
		void sbb_rm32_imm32(void);
		void and_rm32_imm32(void);
		void sub_rm32_imm32(void);
		void xor_rm32_imm32(void);
		void cmp_rm32_imm32(void);

		// 0x83
		void add_rm32_imm8(void);
		void or_rm32_imm8(void);
		void adc_rm32_imm8(void);
		void sbb_rm32_imm8(void);
		void and_rm32_imm8(void);
		void sub_rm32_imm8(void);
		void xor_rm32_imm8(void);
		void cmp_rm32_imm8(void);

		//0xc1
		void shl_rm32_imm8(void);
		void shr_rm32_imm8(void);

		//0xff
		void inc_rm32(void);
		void dec_rm32(void);
		void call_rm32(void);
		void jmp_rm32(void);
		void push_rm32(void);

		//0x0f01
		void lgdt_m32(void);
		void lidt_m32(void);
};

#endif
