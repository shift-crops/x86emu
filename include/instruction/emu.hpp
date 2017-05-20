#ifndef _EMU_INSTR_H
#define _EMU_INSTR_H

#include "common.hpp"
#include "instruction.hpp"

class EmuInstr : protected virtual Instruction {
	public:
		void set_gdtr(uint32_t base, uint16_t limit){ EMU->set_dtreg(GDTR, 0, base, limit); };
		void set_idtr(uint32_t base, uint16_t limit){ EMU->set_dtreg(IDTR, 0, base, limit); };

		void set_ldtr(uint16_t sel);
		uint16_t get_ldtr(void) { return EMU->get_dtreg_selector(LDTR); };
		void set_tr(uint16_t sel);
		uint16_t get_tr(void) { return EMU->get_dtreg_selector(TR); };

		uint8_t type_descriptor(uint16_t sel);
		void switch_task(uint16_t sel);
		void jmpf(uint16_t sel, uint32_t eip);
		void callf(uint16_t sel, uint32_t eip);
		void retf(void);
		void iret(void);

		bool chk_ring(uint8_t DPL);
};

#endif
