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

struct SGRegCache {
	uint32_t base;
	uint32_t limit : 20;

	union {
		uint16_t raw : 12;

		union {
			struct {
				uint8_t : 1;
				uint8_t w : 1;          // 0:r, 1:rw
				uint8_t exd: 1;
				uint8_t : 1;
			} data;
			struct {
				uint8_t : 1;
				uint8_t r : 1;          // 0:x, 1;xr
				uint8_t cnf : 1;
				uint8_t : 1;
			} code;
			struct{
				uint8_t A : 1;
				uint8_t : 2;
				uint8_t segc : 1;
			};
		} type;

		struct {
			uint8_t : 4;
			uint8_t S : 1;
			uint8_t DPL : 2;
			uint8_t P : 1;

			uint8_t AVL : 1;
			uint8_t : 1;
			uint8_t DB : 1;
			uint8_t G : 1;
		};
	} flags;
};

struct SGRegister {
	union {
		uint16_t raw;
		struct {
			uint16_t RPL : 2;
			uint16_t TI : 1;
			uint16_t index : 13;
		};
	};

	SGRegCache cache;
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

	public:
		Processor();
		void dump_regs(void);

		uint32_t get_eip(void){ return eip; };
		uint32_t get_ip(void){ return ip; };
		uint32_t get_gpreg(enum reg32_t n){ ASSERT(n<GPREGS_COUNT); return gpregs[n].reg32; };
		uint16_t get_gpreg(enum reg16_t n){ ASSERT((reg32_t)n<GPREGS_COUNT); return gpregs[n].reg16; };
		uint8_t get_gpreg(enum reg8_t n){ ASSERT((reg32_t)n<GPREGS_COUNT); return n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h; };
		void get_sgreg(enum sgreg_t n, SGRegister *reg){ ASSERT(n<SGREGS_COUNT && reg); *reg = sgregs[n]; };
		//uint16_t get_sgreg(enum sgreg_t n){ return sgregs[n].raw; };
		uint32_t get_dtreg_base(enum dtreg_t n){ ASSERT(n<DTREGS_COUNT); return dtregs[n].base; };
		uint16_t get_dtreg_limit(enum dtreg_t n){ ASSERT(n<DTREGS_COUNT); return dtregs[n].limit; };
		uint16_t get_tr(void){ return tr; };

		void set_eip(uint32_t v){ eip = v; };
		void set_ip(uint16_t v){ ip = v; };
		void set_gpreg(enum reg32_t n, uint32_t v){ ASSERT(n<GPREGS_COUNT); gpregs[n].reg32 = v; };
		void set_gpreg(enum reg16_t n, uint16_t v){ ASSERT((reg32_t)n<GPREGS_COUNT); gpregs[n].reg16 = v; };
		void set_gpreg(enum reg8_t n, uint8_t v){
			ASSERT((reg32_t)n<GPREGS_COUNT); (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) = v; };
		void set_sgreg(enum sgreg_t n, SGRegister *reg){ ASSERT(n<SGREGS_COUNT && reg); sgregs[n] = *reg; };
		void set_dtreg(enum dtreg_t n, uint32_t base, uint16_t limit){
			ASSERT(n<DTREGS_COUNT); dtregs[n].base = base; dtregs[n].limit = limit; };
		void set_tr(uint16_t v){ tr = v; };

		uint32_t update_eip(int32_t v){ return eip += v; };
		uint32_t update_ip(int32_t v){ return ip += v; };
		uint32_t update_gpreg(enum reg32_t n, int32_t v){ ASSERT(n<GPREGS_COUNT); return gpregs[n].reg32 += v; };
		uint16_t update_gpreg(enum reg16_t n, int16_t v){ ASSERT((reg32_t)n<GPREGS_COUNT); return gpregs[n].reg16 += v; };
		uint8_t update_gpreg(enum reg8_t n, int8_t v){
			ASSERT((reg32_t)n<GPREGS_COUNT); return (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) += v; };

		bool is_halt(void){ return halt; };
		void do_halt(bool h){ halt = h; };

		bool is_protected(void) { return CR::is_protected(); };
		bool is_mode32(void) { return sgregs[CS].cache.flags.DB; };
};

#endif
