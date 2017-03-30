#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <stdint.h>
#include "common.hpp"
#include "hardware/hardware.hpp"
#include "access.hpp"
#include "interrupt.hpp"
#include "io.hpp"

class Emulator : public virtual DataAccess, public Interrupt {
	public:
		Emulator(size_t size, uint16_t cs, uint16_t ip) : Hardware(size) { set_sgreg(CS, cs); set_ip(ip); };

		void load_binary(const char* fname, uint32_t addr, size_t size);
};

#endif
