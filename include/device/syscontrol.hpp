#ifndef _SYSCONTROL_H
#define _SYSCONTROL_H

#include <stdint.h>
#include "common.hpp"
#include "device_io.hpp"
#include "hardware/memory.hpp"

class SysControl : public PortIO {
	private:
		Memory *mem;
	public:
		SysControl(Memory *m) { mem = m; };
		uint8_t in8(void);
		void out8(uint8_t v);
};

#endif
