#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <stddef.h>
#include <stdint.h>
#include "../common.hpp"
#include "processor.hpp"
#include "memory.hpp"
#include "eflags.hpp"
#include "io.hpp"

class Emulator : public Processor, public Memory, public IO {
	private:
		Eflags _eflags;

	public:
		Emulator() {};
		Emulator(size_t size, uint32_t eip, uint32_t esp);

		void load_binary(const char* fname, uint32_t addr, size_t size);

		uint8_t get_code8(int index){ return read_mem8(get_eip() + index); };
		uint16_t get_code16(int index){ return read_mem16(get_eip() + index); };
		uint32_t get_code32(int index){ return read_mem32(get_eip() + index); };

		Eflags *p_eflags(void) { return &_eflags; };
};

#endif
