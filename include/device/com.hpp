#ifndef _COM_H
#define _COM_H

#include "common.hpp"
#include "dev_io.hpp"

class COM : public PortIO {
	public:
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
};

#endif
