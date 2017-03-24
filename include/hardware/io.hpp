#ifndef _IO_H
#define _IO_H

#include <stdint.h>
#include "common.hpp" 

class IO {
	public:
		IO();
		uint32_t in_io32(uint16_t addr);
		uint8_t in_io8(uint16_t addr);

		void out_io32(uint16_t addr, uint32_t v);
		void out_io8(uint16_t addr, uint8_t v);
};

#endif
