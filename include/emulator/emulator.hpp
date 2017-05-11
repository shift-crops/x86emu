#ifndef _EMULATOR_H
#define _EMULATOR_H

#include "common.hpp"
#include "access.hpp"
#include "interrupt.hpp"
#include "ui.hpp"
#include "device/devices.hpp"

struct EmuSetting {
	size_t mem_size;
	UISetting uiset;
};

class Emulator : public virtual DataAccess, public Interrupt {
	private:
		UI *ui;
		FDD *fdd;
	public:
		Emulator(EmuSetting set);
		bool is_running(void) const { return ui ? ui->get_status() : false; };
		void stop(void) { delete ui; ui = NULL; };

		void load_binary(const char* fname, uint32_t addr, uint32_t offset, size_t size);
		void insert_fdd(uint8_t slot, const char* disk, bool write) { if(!fdd) return; fdd->insert_disk(slot, disk, write); };
		void eject_fdd(uint8_t slot) { if(!fdd) return; fdd->eject_disk(slot); };
};

#endif
