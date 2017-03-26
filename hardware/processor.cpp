#include <string.h>
#include "hardware/processor.hpp"

Processor::Processor(void){
	memset(gpregs, 0, sizeof(gpregs));
	eip = 0;
	halt = false;
}

void Processor::dump_regs(void){
	const char* gpreg_name[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};

	for(int i=0; i<GPREGS_COUNT; i++)
		MSG("%s = 0x%08x : 0x%04x (0x%02x/0x%02x)\n"
			, gpreg_name[i], gpregs[i].reg32, gpregs[i].reg16, gpregs[i].reg8_h, gpregs[i].reg8_l);
	MSG("EIP = 0x%08x\n", eip);
}
