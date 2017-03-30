#ifndef _COM_H
#define _COM_H

#include <stdint.h>
#include "common.hpp"
#include "device_io.hpp"

class COM : public PortIO {
	public:
		uint8_t in8(void);
		void out8(uint8_t v);
};

#endif
