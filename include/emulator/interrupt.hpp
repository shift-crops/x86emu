#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>
#include "common.hpp"
#include "access.hpp"

#define TYPE_TASK	5
#define TYPE_INTERRUPT	6
#define TYPE_TRAP	7

struct IVT {			// Real Mode
	uint16_t offset;
	uint16_t segment;
};

struct IDT {			// Protected Mode
	uint16_t offset_l;
	uint16_t selector;
	uint8_t : 8;
	uint8_t type : 3;
	uint8_t D : 1;
	uint8_t : 1;
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint16_t offset_h;
};

class Interrupt : public virtual DataAccess {
	public:
		Interrupt();
		void interrupt_hundle(uint8_t n);
		void iret(void);
	private:
		void save_regs(void);
		void restore_regs(void);
};

#endif
