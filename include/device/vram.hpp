#ifndef _VRAM_H
#define _VRAM_H

#include <stdint.h>
#include "common.hpp"
#include "dev_io.hpp"
#include "display.hpp"

class VRAM : public MemoryIO {
	public:
		uint32_t read32(uint32_t offset);
		uint16_t read16(uint32_t offset);
		uint8_t read8(uint32_t offset);
		void write32(uint32_t offset, uint32_t v);
		void write16(uint32_t offset, uint16_t v);
		void write8(uint32_t offset, uint8_t v);
};

#endif
