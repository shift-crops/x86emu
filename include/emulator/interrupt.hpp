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

struct TSS {
	uint32_t T : 1;
	uint32_t : 15;
	uint32_t io_base : 16;
	uint32_t selector : 16;
	uint32_t : 16;
	uint32_t gs : 16;
	uint32_t : 16;
	uint32_t fs : 16;
	uint32_t : 16;
	uint32_t ds : 16;
	uint32_t : 16;
	uint32_t ss : 16;
	uint32_t : 16;
	uint32_t cs : 16;
	uint32_t : 16;
	uint32_t es : 16;
	uint32_t : 16;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t eflags;
	uint32_t eip;
	uint32_t cr3;
	uint32_t ss2 : 16;
	uint32_t : 16;
	uint32_t esp2;
	uint32_t ss1 : 16;
	uint32_t : 16;
	uint32_t esp1;
	uint32_t ss0 : 16;
	uint32_t : 16;
	uint32_t esp0;
	uint32_t backlink : 16;
	uint32_t : 16;
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
