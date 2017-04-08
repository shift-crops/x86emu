#ifndef _MOUSE_H
#define _MOUSE_H

#include "common.hpp"
#include "dev_irq.hpp"
#include "dev_io.hpp"

class Mouse : public IRQ, public PortIO {
	public:
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
		void test(void);
};

#endif
