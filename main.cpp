#include <thread>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <GLFW/glfw3.h>
#include "common.hpp"
#include "instruction/base.hpp"
#include "emulator/emulator.hpp"
#include "emulator/exception.hpp"

#define MEMORY_SIZE (4*MB)

struct Setting {
	const char *image_name;
	uint32_t load_addr;
	size_t load_size;
	uint8_t zoom;
	bool cursor;
};

void run_emulator(const Setting set);
void help(const char *name);

__attribute__((constructor))
void init(void){
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	glfwInit();
}

__attribute__((destructor))
void fini(void){
	glfwTerminate();
}

int main(int argc, char *argv[]){
	Setting set = {
		.image_name = "sample/kernel.img",
		.load_addr = 0x0,
		.load_size = (size_t)-1,
		.zoom = 3,
		.cursor = true,
	};

	char opt;
	struct option long_options[] = {
		{"zoom",	required_argument, NULL, 'z'},
		{"load_addr",	required_argument, NULL, 'a'},
		{"load_size",	required_argument, NULL, 's'},
		{"vm_cursor",	no_argument,       NULL, 'V'},
		{"help",	no_argument,       NULL, 'h'},
		{0, 0, 0, 0}};

	while((opt=getopt_long(argc, argv, DEBUG_OPT"z:a:s:h", long_options, NULL)) != -1){
		switch(opt){
#ifdef DEBUG
			case 'v':
				set_debuglv(optarg);
				break;
#endif
			case 'z':
				set.zoom = atoi(optarg);
				break;
			case 'a':
				set.load_addr = strtol(optarg, NULL, 0);
				break;
			case 's':
				set.load_size = strtol(optarg, NULL, 0);
				break;
			case 'V':
				set.cursor = false;
				break;
			case 'h':
			case '?':
				help(argv[0]);
		}
	}

	if(argc > optind)
		set.image_name = argv[optind];

	/*
	std::thread th1 = std::thread(set);
	std::thread th2 = std::thread(set);
	th1.join();
	th2.join();
	*/
	run_emulator(set);
}

void run_emulator(const Setting set){
	EmuSetting emuset = {
		.mem_size = MEMORY_SIZE,
		.cs = 0xf000, 
		.ip = 0xfff0,
		.uiset = {
			.zoom = set.zoom,
			.cursor = set.cursor,
		},
	};
	Emulator emu = Emulator(emuset);
	InstrData instr;

	Instr16 instr16(&emu, &instr);
	Instr32 instr32(&emu, &instr);

	emu.insert_fdd(0, set.image_name, false);
	emu.load_binary("bios/bios.bin", 0xf0000, 0, 0x2000);
	emu.load_binary("bios/crt0.bin", 0xffff0, 0, 0x10);
	if(set.load_addr)
		emu.load_binary(set.image_name, set.load_addr, 0x200, set.load_size);


	//while(!emu.is_halt()){
	//while(true){
	while(emu.is_running()){
		bool is_protected;
		uint8_t chsz;
		bool chsz_op, chsz_ad;

		memset(&instr, 0, sizeof(InstrData));
		try{
			if(emu.chk_irq())	emu.do_halt(false);
			if(emu.is_halt()){
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
			emu.queue_interrupt(n, true);
			ERROR("Exception %d", n);
		}
		catch(...){
			emu.dump_regs();
			emu.stop();
		}
	}
}

__attribute__((noreturn))
void help(const char *name){
	MSG(	"Simple x86 Emulator\n\n"

		"Usage :\n"
		"\t%s [options] <disk image>\n\n", name);

	MSG(	"Options : \n"
		"\t-z X, --zoom=X\n"
		"\t\tZoom magnification\n\n"
		"\t-a ADDR, --load_addr=ADDR\n"
		"\t\tAddress to preload disk image\n\n"
		"\t-s SIZE, --load_size=SIZE\n"
		"\t\tSize to load (Enabled when 'load_addr' is specified)\n\n"
		"\t--vm_cursor\n"
		"\t\ton Vmware or VirtualBox\n\n"
#ifdef DEBUG
		"\t-v...\n"
		"\t\tverbose level\n\n"
#endif
		"\t-h, --help\n"
		"\t\tshow this help\n");

	_exit(0);
}

