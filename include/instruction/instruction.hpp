#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <map>
#include "common.hpp"
#include "emulator/emulator.hpp"

#define EMU				get_emu()
#define GET_EIP()			EMU->get_eip()
#define GET_IP()			EMU->get_ip()
#define SET_EIP(v)			EMU->set_eip(v)
#define SET_IP(v)			EMU->set_ip(v)
#define UPDATE_EIP(v)			EMU->update_eip(v)
#define UPDATE_IP(v)			EMU->update_ip(v)
#define GET_GPREG(reg)			EMU->get_gpreg(reg)
#define SET_GPREG(reg, v)		EMU->set_gpreg(reg, v)
#define UPDATE_GPREG(reg, v)		EMU->update_gpreg(reg, v)
#define EFLAGS_UPDATE_ADD(v1, v2)	EMU->update_eflags_add(v1, v2)
#define EFLAGS_UPDATE_OR(v1, v2)	EMU->update_eflags_or(v1, v2)
#define EFLAGS_UPDATE_AND(v1, v2)	EMU->update_eflags_and(v1, v2)
#define EFLAGS_UPDATE_SUB(v1, v2)	EMU->update_eflags_sub(v1, v2)
#define EFLAGS_UPDATE_MUL(v1, v2)	EMU->update_eflags_mul(v1, v2)
#define EFLAGS_UPDATE_IMUL(v1, v2)	EMU->update_eflags_imul(v1, v2)
#define EFLAGS_UPDATE_SHL(v1, v2)	EMU->update_eflags_shl(v1, v2)
#define EFLAGS_UPDATE_SHR(v1, v2)	EMU->update_eflags_shr(v1, v2)
#define EFLAGS_CF			EMU->is_carry()
#define EFLAGS_PF			EMU->is_parity()
#define EFLAGS_ZF			EMU->is_zero()
#define EFLAGS_SF			EMU->is_sign()
#define EFLAGS_OF			EMU->is_overflow()
#define EFLAGS_DF			EMU->is_direction()
#define READ_MEM32(addr)		EMU->get_data32(select_segment(), addr)
#define READ_MEM16(addr)		EMU->get_data16(select_segment(), addr)
#define READ_MEM8(addr)			EMU->get_data8(select_segment(), addr)
#define WRITE_MEM32(addr, v)		EMU->put_data32(select_segment(), addr, v)
#define WRITE_MEM16(addr, v)		EMU->put_data16(select_segment(), addr, v)
#define WRITE_MEM8(addr, v)		EMU->put_data8(select_segment(), addr, v)
#define PUSH32(v)			EMU->push32(v)
#define PUSH16(v)			EMU->push16(v)
#define POP32()				EMU->pop32()
#define POP16()				EMU->pop16()

#define PREFIX		(instr->prefix)
#define OPCODE		(instr->opcode)
#define _MODRM		(instr->_modrm)
#define MOD		(instr->modrm.mod)
#define REG		(instr->modrm.reg)
#define RM		(instr->modrm.rm)
#define _SIB		(instr->_sib)
#define SCALE		(instr->sib.scale)
#define INDEX		(instr->sib.index)
#define BASE		(instr->sib.base)
#define DISP32		(instr->disp32)
#define DISP16		(instr->disp16)
#define DISP8		(instr->disp8)
#define IMM32		(instr->imm32)
#define IMM16		(instr->imm16)
#define IMM8		(instr->imm8)
#define PTR16		(instr->ptr16)
#define MOFFS		(instr->moffs)
#define PRE_SEGMENT	(instr->pre_segment)
#define PRE_REPEAT	(instr->pre_repeat)
#define SEGMENT		(instr->segment)

#define MAX_OPCODE	0x200

struct ModRM {  
        uint8_t rm : 3; 
        uint8_t reg : 3; 
        uint8_t mod : 2; 
};

struct SIB {  
        uint8_t base : 3; 
        uint8_t index : 3; 
        uint8_t scale : 2; 
};

enum rep_t { NONE, REPZ, REPNZ };

struct InstrData {
	uint16_t prefix;
	sgreg_t pre_segment;
	rep_t pre_repeat;

	sgreg_t segment;
	uint16_t opcode;
	union {
		uint8_t _modrm;
		struct ModRM modrm;
	};
	union {
		uint8_t _sib;
		struct SIB sib;
	};
	union {
		int8_t disp8;
		int16_t disp16;
		int32_t disp32;
	};
	union {
		int8_t imm8;
		int16_t imm16;
		int32_t imm32;
	};
	int16_t ptr16;
	uint32_t moffs;
};

class Instruction {
	protected:
		InstrData *instr;
		bool chsz_ad;
	private:
		Emulator *emu;
		bool mode32;

	public:
		Instruction() {};
		Instruction(Emulator *e, InstrData *i, bool m) { emu = e; instr = i; mode32 = m; };
	protected:
		Emulator* get_emu(void) { return emu; };
		bool is_mode32(void) { return mode32; };
		sgreg_t select_segment(void) { return instr->prefix ? PRE_SEGMENT : SEGMENT; };
};


class ExecInstr : protected virtual Instruction {
	protected:
		typedef void (ExecInstr::*instrfunc_t)(void);
		//std::map<uint16_t, instrfunc_t> instrfuncs;
		instrfunc_t instrfuncs[MAX_OPCODE];

	public:
		bool exec(void);
	protected:
		//ExecInstr(Emulator *e) : Instruction(e) {}; 
		ExecInstr() { for(int i=0; i<MAX_OPCODE; i++) instrfuncs[i]=NULL; }; 
		void set_rm32(uint32_t value);
		uint32_t get_rm32(void);
		void set_r32(uint32_t value);
		uint32_t get_r32(void);
		void set_moffs32(uint32_t value);
		uint32_t get_moffs32(void);

		void set_rm16(uint16_t value);
		uint16_t get_rm16(void);
		void set_r16(uint16_t value);
		uint16_t get_r16(void);
		void set_moffs16(uint16_t value);
		uint16_t get_moffs16(void);

		void set_rm8(uint8_t value);
		uint8_t get_rm8(void);
		void set_r8(uint8_t value);
		uint8_t get_r8(void);
		void set_moffs8(uint8_t value);
		uint8_t get_moffs8(void);

		uint32_t get_m(void);

		void set_sreg(uint16_t value);
		uint16_t get_sreg(void);

		void set_crn(uint32_t value);
		uint32_t get_crn(void);

	private:
		uint32_t calc_modrm(void);
		uint32_t calc_modrm16(void);
		uint32_t calc_modrm32(void);
		uint32_t calc_sib(void);
};


#define CHK_MODRM 	(1<<0)
#define CHK_IMM32 	(1<<1)
#define CHK_IMM16 	(1<<2)
#define CHK_IMM8 	(1<<3)
#define CHK_PTR16 	(1<<4)
#define CHK_MOFFS 	(1<<5)

#define CHSZ_NONE	0
#define CHSZ_OP		1
#define CHSZ_AD		2

union InstrFlags {
	uint8_t flags;
	struct {
		uint8_t modrm : 1;
		uint8_t imm32 : 1;
		uint8_t imm16 : 1;
		uint8_t imm8 : 1;
		uint8_t ptr16 : 1;
		uint8_t moffs : 1;
		uint8_t moffs8 : 1;
	};
};

class ParseInstr : protected virtual Instruction {
	protected:
		//std::map<uint16_t, InstrFlags> chk;
		InstrFlags chk[MAX_OPCODE];

	public:
		//ParseInstr(Emulator *e) : Instruction(e) {}; 
		void parse(void);
		uint8_t parse_prefix(void);
	private:
		void parse_opcode(void);
		void parse_modrm_sib_disp(void);
		void parse_modrm16(void);
		void parse_modrm32(void);
		void parse_moffs(void);
};

class EmuInstr : protected virtual Instruction {
	public:
		void set_gdtr(uint32_t base, uint16_t limit){ EMU->set_dtreg(GDTR, 0, base, limit); };
		void set_idtr(uint32_t base, uint16_t limit){ EMU->set_dtreg(IDTR, 0, base, limit); };

		void set_ldtr(uint16_t sel);
		uint16_t get_ldtr(void) { return EMU->get_dtreg_selector(LDTR); };
		void set_tr(uint16_t sel);
		uint16_t get_tr(void) { return EMU->get_dtreg_selector(TR); };

		uint8_t type_descriptor(uint16_t sel);
		void switch_task(uint16_t sel);
		void jmpf(uint16_t sel, uint32_t eip);
		void callf(uint16_t sel, uint32_t eip);
		void retf(void);
		void iret(void);

		bool chk_ring(uint8_t DPL);
};

#endif
