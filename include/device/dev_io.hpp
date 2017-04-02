#ifndef _DEV_IO_H
#define _DEV_IO_H

#include <stdint.h>
#include "common.hpp"
//#include "hardware/memory.hpp"

class PortIO {
	public:
		virtual ~PortIO() {};
		virtual uint8_t in8(uint16_t addr) = 0;
		virtual void out8(uint16_t addr, uint8_t v) = 0;
};

class MemoryIO {
/*
	protected:
		uint32_t paddr;
		size_t size;
		Memory *mem;
*/
	public:
		virtual ~MemoryIO() {};

		//void set_mem(Memory *m, uint32_t addr, size_t len) { mem = m; paddr = addr; size = len; };
		virtual uint32_t read32(uint32_t offset) = 0;
		virtual uint16_t read16(uint32_t offset) = 0;
		virtual uint8_t read8(uint32_t offset) = 0;
		virtual void write32(uint32_t offset, uint32_t v) = 0;
		virtual void write16(uint32_t offset, uint16_t v) = 0;
		virtual void write8(uint32_t offset, uint8_t v) = 0;
};

#endif
