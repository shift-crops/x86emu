#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include <stdint.h>
#include "../common.hpp"

enum reg32_t { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, GPREGS_COUNT };
enum reg16_t { AX, CX, DX, BX, SP, BP, SI, DI };
enum reg8_t { AL, CL, DL, BL, AH, CH, DH, BH };
//enum reg8l_t { AL, CL, DL, BL, SPL, BPL, SIL, DIL };
enum segreg_t { CS, DS, ES, SS, FS, GS, SEGREGS_COUNT }; 

union GPRegister {
	uint32_t reg32;
	uint16_t reg16;
	struct {
		uint8_t reg8_l;
		uint8_t reg8_h;
	};
};

class Eflags;

class Processor {
	friend Eflags;

	private:
		GPRegister gpregs[GPREGS_COUNT];

		union {
			uint32_t raw;
			struct {
				uint32_t CF : 1;
				uint32_t : 1;		// 1
				uint32_t PF : 1;
				uint32_t : 1; 		// 0
				uint32_t AF : 1;
				uint32_t : 1; 		// 0
				uint32_t ZF : 1;
				uint32_t SF : 1;
				uint32_t TF : 1;
				uint32_t IF : 1;
				uint32_t DF : 1;
				uint32_t OF : 1;
				uint32_t IOPL : 2;
				uint32_t NT : 1;
				uint32_t : 1; 		// 0
				uint32_t RF : 1;
				uint32_t VM : 1;
				uint32_t AC : 1;
				uint32_t VIF : 1;
				uint32_t VIP : 1;
				uint32_t ID : 1;
			};
		} eflags;

		union {
			uint32_t eip;
			uint16_t ip;
		};

		uint16_t sregs[SEGREGS_COUNT];

		union{
			uint32_t raw;
			struct {
				uint32_t PE : 1;
				uint32_t MP : 1;
				uint32_t EM : 1;
				uint32_t TS : 1;
				uint32_t ET : 1;
				uint32_t NE : 1;
				uint32_t : 10;
				uint32_t WP : 1;
				uint32_t : 1;
				uint32_t AM : 1;
				uint32_t : 10;
				uint32_t NW : 1;
				uint32_t CD : 1;
				uint32_t PG : 1;
			};
		} CR0;

		union{
			uint32_t raw;
		} CR1;

		union{
			uint32_t raw;
		} CR2;

		union{
			uint32_t raw;
			struct {
				uint32_t : 3;
				uint32_t PWT : 1;
				uint32_t PCD : 1;
				uint32_t : 7;
				uint32_t PageDirBase : 20;
			};
		} CR3;

		union{
			uint32_t raw;
			struct {
				uint32_t VME : 1;
				uint32_t PVI : 1;
				uint32_t TSD : 1;
				uint32_t DE : 1;
				uint32_t PSE: 1;
				uint32_t PAE: 1;
				uint32_t MCE: 1;
				uint32_t PGE: 1;
				uint32_t PCE: 1;
				uint32_t OSFXSR: 1;
				uint32_t OSXMMEXCPT: 1;
				uint32_t : 21;
			};
		} CR4;

	public:
		Processor();
		void dump_regs(void);

		uint32_t get_eip(void){ return eip; };
		uint32_t get_eflags(void){ return eflags.raw; };
		uint32_t get_gpreg(enum reg32_t n){ return gpregs[n].reg32; };
		uint16_t get_gpreg(enum reg16_t n){ return gpregs[n].reg16; };
		uint8_t get_gpreg(enum reg8_t n){ return n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h; };

		void set_eip(uint32_t v){ eip = v; };
		void set_eflags(uint32_t v){ eflags.raw = v; };
		void set_gpreg(enum reg32_t n, uint32_t v){ gpregs[n].reg32 = v; };
		void set_gpreg(enum reg16_t n, uint16_t v){ gpregs[n].reg16 = v; };
		void set_gpreg(enum reg8_t n, uint8_t v){ (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) = v; };

		uint32_t update_eip(int32_t v){ return eip += v; };
		uint32_t update_gpreg(enum reg32_t n, int32_t v){ return gpregs[n].reg32 += v; };
		uint16_t update_gpreg(enum reg16_t n, int16_t v){ return gpregs[n].reg16 += v; };
		uint8_t update_gpreg(enum reg8_t n, int8_t v){ return (n<AH ? gpregs[n].reg8_l : gpregs[n-AH].reg8_h) += v; };
};

#endif
