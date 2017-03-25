#ifndef _DATA_ACCESS_H
#define _DATA_ACCESS_H

#include <stdint.h>
#include "common.hpp"
#include "hardware/hardware.hpp"

class DataAccess : public virtual Hardware {
	public:
		DataAccess();

		uint8_t get_code8(int index){ return read_mem8_seg(CS, get_eip() + index); };
		uint16_t get_code16(int index){ return read_mem16_seg(CS, get_eip() + index); };
		uint32_t get_code32(int index){ return read_mem32_seg(CS, get_eip() + index); };

		uint8_t get_data8(uint32_t addr){ return read_mem8_seg(DS, addr); };
		uint16_t get_data16(uint32_t addr){ return read_mem16_seg(DS, addr); };
		uint32_t get_data32(uint32_t addr){ return read_mem32_seg(DS, addr); };
		void put_data8(uint32_t addr, uint8_t v){ write_mem8_seg(DS, addr, v); };
		void put_data16(uint32_t addr, uint16_t v){ write_mem16_seg(DS, addr, v); };
		void put_data32(uint32_t addr, uint32_t v){ write_mem32_seg(DS, addr, v); };

		void push32(uint32_t v);
		uint32_t pop32(void);
		void push16(uint16_t v);
		uint16_t pop16(void);

	private:
		uint32_t trans_v2p_seg(segreg_t seg, uint32_t vaddr);

		uint32_t read_mem32_seg(segreg_t seg, uint32_t addr) { return read_mem32(trans_v2p_seg(seg, addr)); };
		uint16_t read_mem16_seg(segreg_t seg, uint32_t addr) { return read_mem16(trans_v2p_seg(seg, addr)); };
		uint8_t read_mem8_seg(segreg_t seg, uint32_t addr) { return read_mem8(trans_v2p_seg(seg, addr)); };
		void write_mem32_seg(segreg_t seg, uint32_t addr, uint32_t v) { write_mem32(trans_v2p_seg(seg, addr), v); };
		void write_mem16_seg(segreg_t seg, uint32_t addr, uint16_t v) { write_mem16(trans_v2p_seg(seg, addr), v); };
		void write_mem8_seg(segreg_t seg, uint32_t addr, uint8_t v) { write_mem8(trans_v2p_seg(seg, addr), v); };
};

#endif
