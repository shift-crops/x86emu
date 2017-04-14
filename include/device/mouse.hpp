#ifndef _MOUSE_H
#define _MOUSE_H

#include "common.hpp"
#include "dev_irq.hpp"

class Keyboard;
class Mouse : public IRQ {
	private:
		Keyboard *keyboard;
		bool enable;

	public:
		Mouse(Keyboard *kb) { keyboard = kb; enable = false; };
		void command(uint8_t code);
		void send_code(uint8_t code);
};

#endif
