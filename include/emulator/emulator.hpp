#ifndef _EMULATOR_H
#define _EMULATOR_H

#include "common.hpp"
#include "access.hpp"
#include "interrupt.hpp"
#include "ui.hpp"

class Emulator : public virtual DataAccess, public Interrupt {
	private:
		UI *ui;
	public:
		Emulator(size_t size, uint16_t cs, uint16_t ip, const char *disk_name);
		void load_binary(const char* fname, uint32_t addr, uint32_t offset, size_t size);
		bool running(void) const { return ui ? ui->get_status() : false; };
};

#endif
