#include <stdio.h>
#include <string.h>
#include "common.hpp"
#include "instruction/instr_base.hpp"
#include "hardware/emulator.hpp"

#define MEMORY_SIZE (1*MB)

int main(int argc, char *argv[]){
	Emulator emu(MEMORY_SIZE, 0x7c00, 0x100);
	emu.load_binary(argv[1], 0x7c00, 0x200);
	Instr32 instr(&emu);

	while(true){
		instr.parse();
		if(!instr.exec() || !emu.get_eip())
			break;
	}
	emu.dump_mem(emu.get_gpreg(ESP)-0x40, 0x80);
	emu.dump_regs();
}
