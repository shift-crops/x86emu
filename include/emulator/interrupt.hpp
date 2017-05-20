#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <queue>
#include <map>
#include "common.hpp"
#include "access.hpp"
#include "device/pic.hpp"

union IVT {			// Real Mode
	uint32_t raw;
	struct {
		uint16_t offset;
		uint16_t segment;
	};
};

class Interrupt : public virtual DataAccess {
	private:
		std::queue< std::pair<uint8_t, bool> > intr_q;
		PIC *pic_m, *pic_s;

	public:
		void set_pic(PIC *pic, bool master) { (master ? pic_m : pic_s) = pic; };
		void hundle_interrupt(void);
		void queue_interrupt(uint8_t n, bool hard) { intr_q.push(std::make_pair(n, hard)); } ;
		void iret(void);
		bool chk_irq(void);
	private:
		void save_regs(bool chpl, uint16_t cs);
		void restore_regs(void);
};

#endif
