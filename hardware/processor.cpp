#include <string.h>
#include "hardware/processor.hpp"

Processor::Processor(void){
	memset(gpregs, 0, sizeof(gpregs));
	memset(sgregs, 0, sizeof(sgregs));
	eip = 0;
	halt = false;
	mode_protected = false;
}

void Processor::dump_regs(void){
	const char* gpreg_name[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
	const char* sgreg_name[] = { "ES", "CS", "SS", "DS", "FS", "GS" };

	MSG("EIP = 0x%08x\n", eip);
	for(int i=0; i<GPREGS_COUNT; i++)
		MSG("%s = 0x%08x : 0x%04x (0x%02x/0x%02x)\n"
			, gpreg_name[i], gpregs[i].reg32, gpregs[i].reg16, gpregs[i].reg8_h, gpregs[i].reg8_l);
	for(int i=0; i<SGREGS_COUNT; i++)
		MSG("%s=0x%04x ", sgreg_name[i], sgregs[i]);
	MSG("\n");
}
