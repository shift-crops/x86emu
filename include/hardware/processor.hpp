#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "common.hpp"
#include "eflags.hpp"
#include "cr.hpp"

enum reg32_t { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, GPREGS_COUNT };
enum reg16_t { AX, CX, DX, BX, SP, BP, SI, DI };
enum reg8_t { AL, CL, DL, BL, AH, CH, DH, BH };
//enum reg8l_t { AL, CL, DL, BL, SPL, BPL, SIL, DIL };
enum sgreg_t { ES, CS, SS, DS, FS, GS, SGREGS_COUNT }; 
enum dtreg_t { GDTR, LDTR, IDTR, DTREGS_COUNT }; 

union GPRegister {
	uint32_t reg32;
	uint16_t reg16;
	struct {
		uint8_t reg8_l;
		uint8_t reg8_h;
	};
};

union SGRegister {
	uint16_t raw;
	struct {
		uint16_t RPL : 2;
		uint16_t TI : 1;
		uint16_t index : 13;
	};
};

struct DTRegister {
	uint16_t limit;
	uint32_t base;
};

class Processor : public Eflags, public CR {
	private:
		union {
			uint32_t eip;
			uint16_t ip;
		};

		GPRegister gpregs[GPREGS_COUNT];
		SGRegister sgregs[SGREGS_COUNT];
		DTRegister dtregs[DTREGS_COUNT];
		uint16_t tr;

		bool halt;
		bool mode_protected;

	public:
		Processor();
		void dump_regs(void);

		uint32_t get_eip(void){ return eip; };
		uint32_t get_ip(void){ return ip; };
		uint32_t get_gpreg(enum reg32_t n){ return gpregs[n].reg32; };
		uint16_t get_gpreg(enum reg16_t n){ return gpregs[n].reg16; };
		uint8_t get_gpreg(enum reg8_t n){ return n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h; };
		uint16_t get_sgreg(enum sgreg_t n){ return sgregs[n].raw; };
		uint32_t get_dtreg_base(enum dtreg_t n){ return dtregs[n].base; };
		uint16_t get_dtreg_limit(enum dtreg_t n){ return dtregs[n].limit; };
		uint16_t get_tr(void){ return tr; };

		void set_eip(uint32_t v){ eip = v; };
		void set_ip(uint16_t v){ ip = v; };
		void set_gpreg(enum reg32_t n, uint32_t v){ gpregs[n].reg32 = v; };
		void set_gpreg(enum reg16_t n, uint16_t v){ gpregs[n].reg16 = v; };
		void set_gpreg(enum reg8_t n, uint8_t v){ (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) = v; };
		void set_sgreg(enum sgreg_t n, uint16_t v){ sgregs[n].raw = v; };
		void set_dtreg(enum dtreg_t n, uint32_t base, uint16_t limit){ dtregs[n].base = base; dtregs[n].limit = limit; };
		void set_tr(uint16_t v){ tr = v; };

		uint32_t update_eip(int32_t v){ return eip += v; };
		uint32_t update_ip(int32_t v){ return ip += v; };
		uint32_t update_gpreg(enum reg32_t n, int32_t v){ return gpregs[n].reg32 += v; };
		uint16_t update_gpreg(enum reg16_t n, int16_t v){ return gpregs[n].reg16 += v; };
		uint8_t update_gpreg(enum reg8_t n, int8_t v){ return (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) += v; };

		void flush(void){ mode_protected = CR::is_protected(); };
		bool is_halt(void){ return halt; };
		void do_halt(bool h){ halt = h; };

		bool is_protected(void) { return mode_protected; };
};

#endif
