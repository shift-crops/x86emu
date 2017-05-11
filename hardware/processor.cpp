#include <string.h>
#include "hardware/processor.hpp"

Processor::Processor(void){
	memset(gpregs, 0, sizeof(gpregs));
	memset(sgregs, 0, sizeof(sgregs));

	set_eip(0x0000fff0);
	set_sgreg(CS, 0xf000);
	set_crn(0, 0x60000010);
	set_eflags(0x00000002);

	dtregs[IDTR].base  = 0x0000;
	dtregs[IDTR].limit = 0xffff;
	dtregs[GDTR].base  = 0x0000;
	dtregs[GDTR].limit = 0xffff;
	dtregs[LDTR].base  = 0x0000;
	dtregs[LDTR].limit = 0xffff;

	halt = false;
	mode_protected = false;
}

void Processor::dump_regs(void){
	int i;
	const char* gpreg_name[] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
	const char* sgreg_name[] = { "ES", "CS", "SS", "DS", "FS", "GS" };

	MSG("EIP = 0x%08x\n", eip);
	for(i=0; i<GPREGS_COUNT; i++)
		MSG("%s = 0x%08x : 0x%04x (0x%02x/0x%02x)\n"
			, gpreg_name[i], gpregs[i].reg32, gpregs[i].reg16, gpregs[i].reg8_h, gpregs[i].reg8_l);
	MSG("EFLAGS = 0x%08x\n", get_eflags());
	for(i=0; i<SGREGS_COUNT; i++)
		MSG("%s=0x%04x ", sgreg_name[i], *((uint16_t*)&sgregs[i]));
	MSG("\n");

	for(i=0; i<5; i++)
		MSG("CR%d=0x%08x ", i, get_crn(i));
	MSG("\n");
}
