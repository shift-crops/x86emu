#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stddef.h>
#include "common.hpp"
#include "processor.hpp"
#include "memory.hpp"
#include "eflags.hpp"

class Emulator;

class Hardware : public Processor, public Memory {
	friend Emulator;

	public:
		Hardware(size_t size = 0) : Memory(size) {};
};

#endif
