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
	size_t mem_size;
	const char *image_name;
	uint32_t load_addr;
	size_t load_size;
	bool ui_enable;
	uint8_t ui_zoom;
	bool ui_full;
	bool ui_vm;
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
		.mem_size = MEMORY_SIZE, 
		.image_name = "sample/kernel.img",
		.load_addr = 0x0,
		.load_size = (size_t)-1,
		.ui_enable = true,
		.ui_zoom = 3,
		.ui_full = false,
		.ui_vm = false,
	};

	char opt;
	struct option long_options[] = {
		{"memory",	required_argument, NULL, 1},
		{"zoom",	required_argument, NULL, 2},
		{"load_addr",	required_argument, NULL, 3},
		{"load_size",	required_argument, NULL, 4},
		{"full",	no_argument,       NULL, 5},
		{"VM",		no_argument,       NULL, 6},
		{"no_graphic",	no_argument,       NULL, 7},
		{"help",	no_argument,       NULL, 'h'},
		{0, 0, 0, 0}};

	while((opt=getopt_long(argc, argv, DEBUG_OPT"m:z:a:s:h", long_options, NULL)) != -1){
		switch(opt){
			uint32_t v;
			case 'm':
			case 1:
				v = atoi(optarg);

				if(v)
					set.mem_size = v*MB;
				else
					WARN("memory size is zero");
				break;
			case 'z':
			case 2:
				v = atoi(optarg);
				if(v)
					set.ui_zoom = v;
				else
					WARN("zoom error");
				break;
			case 'a':
			case 3:
				set.load_addr = strtol(optarg, NULL, 0);
				break;
			case 's':
			case 4:
				set.load_size = strtol(optarg, NULL, 0);
				break;
			case 5:
				set.ui_full = true;
				break;
			case 6:
				set.ui_vm = true;
				break;
			case 7:
				set.ui_enable = false;
				break;
#ifdef DEBUG
			case 'v':
				set_debuglv(optarg);
				break;
#endif
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
		.mem_size = set.mem_size,
		.uiset = {
			.enable = set.ui_enable,
			.zoom = set.ui_zoom,
			.full = set.ui_full,
			.vm = set.ui_vm,
		},
	};
	Emulator emu = Emulator(emuset);
	InstrData instr;

	Instr16 instr16(&emu, &instr);
	Instr32 instr32(&emu, &instr);

	if(!emu.insert_floppy(0, set.image_name, false)){
		WARN("cannot load image '%s'", set.image_name);
		return;
	}

	emu.load_binary("bios/bios.bin", 0xf0000, 0, 0x2000);
	emu.load_binary("bios/crt0.bin", 0xffff0, 0, 0x10);
	if(set.load_addr)
		emu.load_binary(set.image_name, set.load_addr, 0x200, set.load_size);


	//while(!emu.is_halt()){
	//while(true){
	while(emu.is_running()){
		bool is_mode32;
		uint8_t prefix;
		bool chsz_op, chsz_ad;

		memset(&instr, 0, sizeof(InstrData));
		try{
			if(emu.chk_irq())	emu.do_halt(false);
			if(emu.is_halt()){
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			emu.hundle_interrupt();

			is_mode32 = emu.is_mode32();
			prefix = (is_mode32 ? instr32.parse_prefix() : instr16.parse_prefix());
			chsz_op = prefix & CHSZ_OP;
			chsz_ad = prefix & CHSZ_AD;

			if(is_mode32 ^ chsz_op){
				instr32.set_chsz_ad(!(is_mode32 ^ chsz_ad));
				instr32.parse();
				instr32.exec();
			}
			else{
				instr16.set_chsz_ad(is_mode32 ^ chsz_ad);
				instr16.parse();
				instr16.exec();
			}
		}
		catch(exception_t n){
			emu.queue_interrupt(n, true);
			INFO(3, "Exception %d", n);
			//ERROR("Exception %d", n);
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
		"\t-m MB, --memory=MB\n"
		"\t\tMemory size (MB)\n\n"
		"\t-z X, --zoom=X\n"
		"\t\tZoom magnification\n\n"
		"\t-a ADDR, --load_addr=ADDR\n"
		"\t\tAddress to preload disk image\n\n"
		"\t-s SIZE, --load_size=SIZE\n"
		"\t\tSize to load (Enabled when 'load_addr' is specified)\n\n"
		"\t--full\n"
		"\t\tfull screen\n\n"
		"\t--VM\n"
		"\t\trunning on VMware or VirtualBox\n\n"
#ifdef DEBUG
		"\t-v...\n"
		"\t\tverbose level\n\n"
#endif
		"\t-h, --help\n"
		"\t\tshow this help\n");

	_exit(0);
}

