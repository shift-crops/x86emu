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
#define EFLAGS_CF			EMU->is_carry()
#define EFLAGS_PF			EMU->is_parity()
#define EFLAGS_ZF			EMU->is_zero()
#define EFLAGS_SF			EMU->is_sign()
#define EFLAGS_OF			EMU->is_overflow()
#define READ_MEM32(addr)		EMU->get_data32(DS, addr)
#define READ_MEM16(addr)		EMU->get_data16(DS, addr)
#define READ_MEM8(addr)			EMU->get_data8(DS, addr)
#define WRITE_MEM32(addr, v)		EMU->put_data32(DS, addr, v)
#define WRITE_MEM16(addr, v)		EMU->put_data16(DS, addr, v)
#define WRITE_MEM8(addr, v)		EMU->put_data8(DS, addr, v)
#define PUSH32(v)			EMU->push32(v)
#define PUSH16(v)			EMU->push16(v)
#define POP32()				EMU->pop32()
#define POP16()				EMU->pop16()

#define PREFIX	(instr.prefix)
#define OPCODE	(instr.opcode)
#define _MODRM	(instr._modrm)
#define MOD	(instr.modrm.mod)
#define REG	(instr.modrm.reg)
#define RM	(instr.modrm.rm)
#define _SIB	(instr._sib)
#define SCALE	(instr.sib.scale)
#define INDEX	(instr.sib.index)
#define BASE	(instr.sib.base)
#define DISP32	(instr.disp32)
#define DISP16	(instr.disp16)
#define DISP8	(instr.disp8)
#define IMM32	(instr.imm32)
#define IMM16	(instr.imm16)
#define IMM8	(instr.imm8)
#define PTR16	(instr.ptr16)
#define SEGMENT	(segment)

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

class Instruction {
	protected:
		struct {
			uint16_t prefix;
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
		} instr;

		sgreg_t segment;
		bool mode_protected;
	private:
		Emulator *emu;

	public:
		Instruction() {};
		Instruction(Emulator *e, bool p) { emu = e; mode_protected = p; };
	protected:
		Emulator* get_emu() { return emu; };
		bool is_protected() { return mode_protected; };
};


class ExecInstr : protected virtual Instruction {
	protected:
		typedef void (ExecInstr::*instrfunc_t)(void);
		std::map<uint16_t, instrfunc_t> instrfuncs;

	public:
		bool exec(void);
	protected:
		//ExecInstr(Emulator *e) : Instruction(e) {}; 
		void set_rm32(uint32_t value);
		uint32_t get_rm32(void);
		void set_r32(uint32_t value);
		uint32_t get_r32(void);

		void set_rm16(uint16_t value);
		uint16_t get_rm16(void);
		void set_r16(uint16_t value);
		uint16_t get_r16(void);

		void set_rm8(uint8_t value);
		uint8_t get_rm8(void);
		void set_r8(uint8_t value);
		uint8_t get_r8(void);

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


#define CHK_MODRM 	1
#define CHK_IMM32 	2
#define CHK_IMM16 	4
#define CHK_IMM8 	8
#define CHK_PTR16 	16

union InstrFlags {
	uint8_t flags;
	struct {
		uint8_t modrm : 1;
		uint8_t imm32 : 1;
		uint8_t imm16 : 1;
		uint8_t imm8 : 1;
		uint8_t ptr16 : 1;
	};
};

class ParseInstr : protected virtual Instruction {
	protected:
		std::map<uint16_t, InstrFlags> chk;

	public:
		//ParseInstr(Emulator *e) : Instruction(e) {}; 
		bool parse(void);
	private:
		bool parse_prefix_opcode(void);
		void parse_modrm_sib_disp(void);
};

#endif
