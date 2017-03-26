#ifndef _DEVICE_IO_H
#define _DEVICE_IO_H

#include <stdint.h>
#include "common.hpp"

class DeviceIO {
	public:
		virtual ~DeviceIO() {};
		virtual uint8_t in8(void) = 0;
		virtual void out8(uint8_t v) = 0;
};

#endif
