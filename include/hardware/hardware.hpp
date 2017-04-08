#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stddef.h>
#include "common.hpp"
#include "processor.hpp"
#include "memory.hpp"
#include "eflags.hpp"
#include "io.hpp"

class Hardware : public Processor, public Memory, public IO {
	public:
		Hardware(size_t size = 0) : Memory(size), IO(this) {};
};

#endif
