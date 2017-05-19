#ifndef _DMA_H
#define _DMA_H

#include "common.hpp"
#include "dev_drq.hpp"
#include "dev_io.hpp"

class DMA : public DRQ, public PortIO {
	private:
		DRQ *drq[4];

		uint16_t adrr[4];
		uint8_t xpadrr[4];
		uint16_t cntr[4];

		union {
			uint8_t raw;
			struct {
				uint8_t NMT : 1;
				uint8_t ADHE : 1;
				uint8_t COND : 1;
				uint8_t COMP : 1;
				uint8_t PRIO : 1;
				uint8_t EXTW : 1;
				uint8_t DRQP : 1;
				uint8_t DACKP : 1;
			};
		} cmnr;

		union {
			uint8_t raw;
			struct {
				uint8_t SEL : 2;
				uint8_t TRA : 2;
				uint8_t AUTO : 1;
				uint8_t DOWN : 1;
				uint8_t MOD : 2;
			};
		} modr;

		union {
			uint8_t raw;
			struct {
				uint8_t SEL : 2;
				uint8_t REQ : 1;
			};
		} reqr;

		union {
			uint8_t raw;
			struct {
				uint8_t SEL : 2;
				uint8_t MASK : 1;
			};
		} scmr;

		union {
			uint8_t raw;
			struct {
				uint8_t CH0 : 1;
				uint8_t CH1 : 1;
				uint8_t CH2 : 1;
				uint8_t CH3 : 1;
			};
		} amr;

		union {
			uint8_t raw;
			struct {
				uint8_t TC0 : 1;
				uint8_t TC1 : 1;
				uint8_t TC2 : 1;
				uint8_t TC3 : 1;
				uint8_t CH0 : 1;
				uint8_t CH1 : 1;
				uint8_t CH2 : 1;
				uint8_t CH3 : 1;
			};
		} statr;

	public:
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
};

#endif
