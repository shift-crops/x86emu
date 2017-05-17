#ifndef _PIT_H
#define _PIT_H

#include "common.hpp"
#include <thread>
#include "dev_irq.hpp"
#include "dev_io.hpp"

struct Timer {
	uint8_t mode;
	uint16_t count;
	uint16_t def;

	bool running;
	std::thread th;
};

class PIT : public IRQ, public PortIO {
	private:
		union {
			uint8_t raw;
			struct {
				uint8_t BCD : 1;
				uint8_t mode : 3;
				uint8_t RL : 2;
				uint8_t SC : 2;
			};
		} cwr;

		Timer timer[3];

	public:
		PIT();
		~PIT();
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
		//bool chk_intreq(void);
		void counter(Timer *t);
};

#endif
