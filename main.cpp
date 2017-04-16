#include <thread>
#include <stdio.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include "common.hpp"
#include "instruction/base.hpp"
#include "emulator/emulator.hpp"
#include "emulator/exception.hpp"

#define MEMORY_SIZE (1*MB)

void run_emulator(const char *image_name, bool preload);

int main(int argc, char *argv[]){
	glfwInit();

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	/*
	std::thread th1 = std::thread(run_emulator, argc<2 ? "sample/kernel.img": argv[1], false);
	std::thread th2 = std::thread(run_emulator, argc<2 ? "sample/kernel.img": argv[1], false);
	th1.join();
	th2.join();
	*/
	run_emulator(argc<2 ? "sample/kernel.img": argv[1], false);

	glfwTerminate();
}

void run_emulator(const char *image_name, bool preload){
	Emulator emu = Emulator(MEMORY_SIZE, 0xf000, 0xfff0, image_name);
	InstrData instr;

	Instr16 instr16(&emu, &instr);
	Instr32 instr32(&emu, &instr);

	emu.load_binary("bios/bios.bin", 0xf0000, 0, 0x800);
	emu.load_binary("bios/hankaku.chr", 0xfc000, 0, 0x1000);
	emu.load_binary("bios/crt0.bin", 0xffff0, 0, 0x10);
	if(preload)
		emu.load_binary(image_name, 0x10000, 0x200, 0x2000);


	//while(!emu.is_halt()){
	//while(true){
	while(emu.running()){
		bool is_protected;
		uint8_t chsz;
		bool chsz_op, chsz_ad;

		memset(&instr, 0, sizeof(InstrData));
		try{
			if(emu.chk_irq())	emu.do_halt(false);
			if(emu.is_halt()){
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				continue;
			}
			emu.hundle_interrupt();

			is_protected = emu.is_protected();
			chsz = (is_protected ? instr32.parse_prefix() : instr16.parse_prefix());
			chsz_op = chsz & CHSZ_OP;
			chsz_ad = chsz & CHSZ_AD;

			if(is_protected ^ chsz_op){
				instr32.set_chsz_ad(!(is_protected ^ chsz_ad));
				instr32.parse();
				instr32.exec();
			}
			else{
				instr16.set_chsz_ad(is_protected ^ chsz_ad);
				instr16.parse();
				instr16.exec();
			}
		}
		catch(exception_t n){
			emu.dump_regs();
			ERROR("Exception %d", n);
			emu.queue_interrupt(n, true);
		}
		/*
		emu.dump_regs();
		if((emu.get_sgreg(SS)<<4)+emu.get_gpreg(ESP)>0x40)
			emu.dump_mem((emu.get_sgreg(SS)<<4)+emu.get_gpreg(ESP)-0x40, 0x80);
		MSG("\n");
		*/
	}
}
