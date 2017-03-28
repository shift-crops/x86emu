#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>
#include "common.hpp"
#include "access.hpp"

union IVT {			// Real Mode
	uint32_t raw;
	struct {
		uint16_t offset;
		uint16_t segment;
	};
};

class Interrupt : public virtual DataAccess {
	public:
		Interrupt();
		void interrupt_hundle(uint8_t n, bool hd_exp);
		void iret(void);
	private:
		void save_regs(bool chpl);
		void restore_regs(void);
};

#endif
