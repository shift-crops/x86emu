#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "common.hpp"
#include "dev_irq.hpp"
#include "dev_io.hpp"

class Keyboard : public IRQ, public PortIO {
	private:
		uint32_t code;

	public:
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
		bool send_code(uint32_t scancode);
};

#endif
