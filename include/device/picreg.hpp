#ifndef _PICREG_H
#define _PICREG_H

#include <stdint.h>
#include "common.hpp"
#include "dev_io.hpp"
#include "emulator/pic.hpp"

class PICReg : public PortIO {
	private:
		PIC *pic;
	public:
		PICReg(PIC *p) { if(!p) ERROR(""); pic = p; };
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
	private:
		void set_command(uint8_t v);
		void set_data(uint8_t v);
};

#endif
