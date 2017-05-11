#include <stdio.h>
#include "emulator/emulator.hpp"
#include "device/devices.hpp"

Emulator::Emulator(EmuSetting set) : Hardware(set.mem_size) {
	PIC *pic_m, *pic_s;
	PIT *pit;
	SysControl *syscon;
	COM *com;
	VGA *vga;
	Keyboard *kb;

	pic_m = new PIC();
	pic_s = new PIC(pic_m);
	set_pic(pic_m, true);
	set_pic(pic_s, false);

	ui	= new UI(this, set.uiset);
	pit	= new PIT();
	fdd	= new FDD();
	syscon	= new SysControl(this);
	com	= new COM();
	vga	= ui->get_vga();
	kb	= ui->get_keyboard();

	pic_m->set_irq(0, pit);
	pic_m->set_irq(1, kb);
	pic_m->set_irq(2, pic_s);
	pic_m->set_irq(6, fdd);
	pic_s->set_irq(4, kb->get_mouse());

	set_portio(0x020, 2, pic_m);		// 0x20, 0x21
	set_portio(0x040, 4, pit);		// 0x40, 0x41, 0x42, 0x43
	set_portio(0x060, 1, kb);		// 0x60
	set_portio(0x064, 1, kb);		// 0x64
	set_portio(0x0a0, 2, pic_s);		// 0xa0, 0xa1
	set_portio(0x092, 1, syscon);		// 0x92
	set_portio(0x3b4, 2, vga->get_crt());	// 0x3b4, 0x3b5
	set_portio(0x3ba, 1, vga);		// 0x3ba
	set_portio(0x3c0, 2, vga->get_attr());	// 0x3c0, 0x3c1
	set_portio(0x3c2, 2, vga);		// 0x3c2, 0x3c3
	set_portio(0x3c4, 2, vga->get_seq());	// 0x3c4, 0x3c5
	set_portio(0x3c6, 4, vga->get_dac());	// 0x3c6, 0x3c7, 0x3c8, 0x3c9
	set_portio(0x3cc, 1, vga);		// 0x3cc
	set_portio(0x3ce, 2, vga->get_gc());	// 0x3ce, 0x3cf
	set_portio(0x3d4, 2, vga->get_crt());	// 0x3d4, 0x3d5
	set_portio(0x3da, 1, vga);		// 0x3da
	set_portio(0x3f0, 8, fdd);		// 0x3f0, 0x3f1, 0x3f2, 0x3f3, 0x3f4, 0x3f5, 0x3f7
	set_portio(0x3f8, 1, com);		// 0x3f8
	set_memio(0xa0000, 0x20000, vga);

	//set_sgreg(CS, set.cs);
	//set_ip(set.ip);
}

void Emulator::load_binary(const char* fname, uint32_t addr, uint32_t offset, size_t size){
	FILE *fp;
	uint8_t *buf;

	fp = fopen(fname, "rb");
	if(!fp)
		return;

	if((int32_t)size < 0){
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
	}

	buf = new uint8_t[size];
	fseek(fp, offset, SEEK_SET);
	fread(buf, 1, size, fp);
	fclose(fp);

	write_data(addr, buf, size);

	delete[] buf;
}
