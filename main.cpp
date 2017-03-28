#include <stdio.h>
#include <string.h>
#include "common.hpp"
#include "instruction/base.hpp"
#include "emulator/emulator.hpp"

#define MEMORY_SIZE (1*MB)

int main(int argc, char *argv[]){
	Emulator emu(MEMORY_SIZE, 0xf000, 0xfff0);
	Instr16 instr16(&emu);
	Instr32 instr32(&emu);

	emu.load_binary("bios/bios", 0xffff0, 0x10);
	emu.load_binary(argv[1], 0x7c00, 0x200);

	while(!emu.is_halt() && emu.get_eip()){
		bool is_protected = emu.is_protected();

		while(!(is_protected ? instr32.parse() : instr16.parse()))
			is_protected ^= true;

		if(is_protected)
			instr32.exec();
		else
			instr16.exec();
	}
	emu.dump_mem(emu.get_gpreg(ESP)-0x40, 0x80);
	emu.dump_regs();
}
