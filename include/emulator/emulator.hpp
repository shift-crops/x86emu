#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <stdint.h>
#include "common.hpp"
#include "hardware/hardware.hpp"
#include "access.hpp"
#include "interrupt.hpp"

class Emulator : public Interrupt {
	public:
		Emulator(size_t size, uint32_t eip, uint16_t cs) : Hardware(size) { set_eip(eip); set_segreg(CS, cs); };

		void load_binary(const char* fname, uint32_t addr, size_t size);
};

#endif
