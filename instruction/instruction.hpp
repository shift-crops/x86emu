#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "../common.hpp"
#include "../hardware/emulator.hpp"

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
#define DISP8	(instr.disp8_s)
#define IMM32	(instr.imm32)
#define IMM8	(instr.imm8_s)

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
		Emulator *emu;

		struct {
			//uint32_t prefix;
			uint32_t opcode : 24;
			union {
				uint8_t _modrm;
				struct ModRM modrm;
			};
			union {
				uint8_t _sib;
				struct SIB sib;
			};
			union {
				int8_t disp8_s;
				int32_t disp32;
			};
			union {
				int8_t imm8_s;
				int32_t imm32;
			};
		} instr;
	public:
		Instruction(Emulator *e = NULL) { emu = e; };
};


class ExecInstr : protected virtual Instruction {
	protected:
		typedef void (ExecInstr::*instrfunc_t)(void);
		instrfunc_t instrfuncs[256];

	public:
		bool exec(void);
	protected:
		void set_rm32(uint32_t value);
		uint32_t get_rm32(void);
		void set_r32(uint32_t value);
		uint32_t get_r32(void);
		void set_rm8(uint8_t value);
		uint32_t get_rm8(void);
		void set_r8(uint8_t value);
		uint32_t get_r8(void);
		void push32(uint32_t value);
		uint32_t pop32(void);
	private:
		virtual void init_instr() = 0;
		uint32_t calc_modrm(void);
		uint32_t calc_sib(void);
};


#define CHK_MODRM 	1
#define CHK_IMM32 	2
#define CHK_IMM8 	4

class ParseInstr : protected virtual Instruction {
	protected:
		union {
			uint8_t flags;
			struct {
				uint8_t modrm : 1;
				uint8_t imm32 : 1;
				uint8_t imm8 : 1;
			};
		} chk[256];

	public:
		void parse(void);
	private:
		void parse_modrm_sib_disp(void);
};

#endif
