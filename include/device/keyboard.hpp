#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "common.hpp"
#include "dev_irq.hpp"
#include "dev_io.hpp"
#include "mouse.hpp"

struct CCB {
	uint8_t KIE : 1;
	uint8_t MIE : 1;
	uint8_t SYSF : 1;
	uint8_t IGNLK : 1;
	uint8_t KE : 1;
	uint8_t ME : 1;
	uint8_t XLATE : 1;
};

class Keyboard : public IRQ, public PortIO {
	friend Mouse;
	private:
		Mouse *mouse;
		uint8_t mode;

		union {
			uint8_t raw;
			struct {
				uint8_t OBF : 1;
				uint8_t IBF : 1;
				uint8_t F0 : 1;
				uint8_t F1 : 1;
				uint8_t ST4 : 1;
				uint8_t ST5 : 1;
				uint8_t ST6 : 1;
				uint8_t ST7 : 1;
			};
		} kcsr;

		uint8_t out_buf;
		uint8_t in_buf;
		uint8_t controller_ram[0x20];
		CCB *ccb = (CCB*)&controller_ram[0];

	public:
		Keyboard() { mouse = new Mouse(this); kcsr.raw = 0; };
		Mouse *get_mouse(void) const { return mouse; };

		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
		void command(uint8_t v);
		uint8_t read_outbuf(void);
		void write_outbuf(uint8_t v);
		void send_code(uint32_t scancode);
};

#endif
