#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stddef.h>
#include "common.hpp"
#include "processor.hpp"
#include "memory.hpp"
#include "eflags.hpp"
#include "io.hpp"

class Emulator;

class Hardware : public Processor, public Memory, public IO {
	friend Emulator;

	private:
		Eflags _eflags;

	public:
		Hardware(size_t size) : Memory(size) { _eflags = Eflags(this); };
};

#endif
