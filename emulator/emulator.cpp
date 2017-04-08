#include <stdio.h>
#include <stdint.h>
#include "emulator/emulator.hpp"
#include "emulator/pic.hpp"
#include "emulator/ui.hpp"
#include "device/devices.hpp"

Emulator::Emulator(size_t size, uint16_t cs, uint16_t ip, const char *disk_name) : Hardware(size) {
	PIC *pic_m, *pic_s;
	PIT *pit;
	FDD *fdd;
	Display *disp;
	Keyboard *keyboard;
	Mouse *mouse;

	pic_m = new PIC();
	pic_s = new PIC(pic_m);
	set_pic(pic_m, true);
	set_pic(pic_s, false);

	pit = new PIT();
	fdd = new FDD();
	fdd->insert_disk(0, disk_name, false);

	disp = new Display();
	keyboard = new Keyboard();
	mouse = new Mouse();
	new UI(disp, keyboard, mouse);

	pic_m->set_irq(0, pit);
	pic_m->set_irq(1, keyboard);
	pic_m->set_irq(2, pic_s);
	pic_m->set_irq(6, fdd);

	pic_s->set_irq(4, mouse);

	set_portio(0x20, new PICReg(pic_m));	// 0x20, 0x21
	set_portio(0x40, pit);			// 
	set_portio(0x60, keyboard);		// 
	set_portio(0xa0, new PICReg(pic_s));	// 0xa0, 0xa1
	set_portio(0x90, new SysControl(this));	// 0x92
	set_portio(0x3f0, fdd);
	set_portio(0x3f4, fdd);
	set_portio(0x3f8, new COM());		// 0x3f8
	set_memio(0xa0000, 0x10000, disp->get_vram(MODE_GRAPHIC));
	set_memio(0xb0000, 0x8000,  disp->get_vram(MODE_TEXT_MONO));
	set_memio(0xb8000, 0x8000,  disp->get_vram(MODE_TEXT_COLOR));

	set_sgreg(CS, cs);
	set_ip(ip);
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
