#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "emulator.hpp"

Emulator::Emulator(size_t size, uint32_t eip, uint32_t esp) : Memory(size){
	_eflags = Eflags(this);

	set_eip(eip);
	set_gpreg(ESP, esp);
}

void Emulator::load_binary(const char* fname, uint32_t addr, size_t size){
	FILE *fp;
	uint8_t *buf;

	fp = fopen(fname, "rb");
	if(!fp)
		return;

	buf = new uint8_t[size];
	fread(buf, 1, size, fp);
	fclose(fp);

	write_data(addr, buf, size);

	delete[] buf;
}
