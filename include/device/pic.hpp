#ifndef _PIC_H
#define _PIC_H

#include "common.hpp"
#include "dev_irq.hpp"
#include "dev_io.hpp"

#define MAX_IRQ 8

union OCW2 {
	uint8_t raw;
	struct {
		uint8_t L : 1;
		uint8_t : 2;
		uint8_t EOI : 1;
		uint8_t SL : 1;
		uint8_t R : 1;
	};
};

class PIC : public IRQ, public PortIO {
	private:
		PIC *pic_m;
		IRQ *irq[MAX_IRQ];
		uint8_t irr;
		uint8_t isr;
		uint8_t imr;

		union {
			uint8_t raw;
			struct {
				uint8_t IC4 : 1;
				uint8_t SNGL : 1;
				uint8_t ADI : 1;
				uint8_t LTIM : 1;
				uint8_t : 1;
				uint8_t IVA_l : 3;	// MCS-80/85
			};
		} ic1;

		union {
			uint8_t raw;
			struct {
				uint8_t IVA_h : 3;	// MCS-80/85
				uint8_t IVA_x86 : 5;
			};
		} ic2;

		union {
			uint8_t raw;
			struct {
				uint8_t S0 : 1;
				uint8_t S1 : 1;
				uint8_t S2 : 1;
				uint8_t S3 : 1;
				uint8_t S4 : 1;
				uint8_t S5 : 1;
				uint8_t S6 : 1;
				uint8_t S7 : 1;
			};

			struct {
				uint8_t ID : 3;
			};
		} ic3;

		union {
			uint8_t raw;
			struct {
				uint8_t PM : 1;
				uint8_t AEOI : 1;
				uint8_t MS : 1;
				uint8_t BUF : 1;
				uint8_t SFNM : 1;
			};
		} ic4;

		uint8_t init_icn;

	public:
		PIC(PIC* master = NULL);
		void set_irq(uint8_t n, IRQ *dev) { if(n<MAX_IRQ) irq[n] = dev; else ERROR("IRQ out of bound : %d", n); };
		int8_t get_nintr(void);
		bool chk_intreq(void);
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
	private:
		bool chk_m2s_pic(uint8_t n) { return !ic1.SNGL && !pic_m && ic3.raw&(1<<n); };	// this : master
		void set_command(uint8_t v);
		void set_data(uint8_t v);
};

#endif
