#ifndef _DATA_ACCESS_H
#define _DATA_ACCESS_H

#include "common.hpp"
#include <vector>
#include "hardware/hardware.hpp"

struct PDE {
	uint32_t P : 1;
	uint32_t RW : 1;
	uint32_t US : 1;
	uint32_t PWT : 1;
	uint32_t PCD : 1;
	uint32_t A : 1;
	uint32_t : 1;
	uint32_t PS : 1;
	uint32_t G : 1;
	uint32_t : 3;
	uint32_t ptbl_base : 20;
};

struct PTE {
	uint32_t P : 1;
	uint32_t RW : 1;
	uint32_t US : 1;
	uint32_t PWT : 1;
	uint32_t PCD : 1;
	uint32_t A : 1;
	uint32_t D : 1;
	uint32_t PAT : 1;
	uint32_t G : 1;
	uint32_t : 3;
	uint32_t page_base : 20;
};

enum acsmode_t { MODE_READ, MODE_WRITE, MODE_EXEC };

class DataAccess : public virtual Hardware {
	private:
		std::vector<PTE*> tlb;

	public:
		void set_segment(sgreg_t seg, uint16_t v);
		uint16_t get_segment(sgreg_t seg);

		uint8_t get_data8(sgreg_t seg, uint32_t addr){ return read_mem8_seg(seg, addr); };
		uint16_t get_data16(sgreg_t seg, uint32_t addr){ return read_mem16_seg(seg, addr); };
		uint32_t get_data32(sgreg_t seg, uint32_t addr){ return read_mem32_seg(seg, addr); };

		void put_data8(sgreg_t seg, uint32_t addr, uint8_t v){ write_mem8_seg(seg, addr, v); };
		void put_data16(sgreg_t seg, uint32_t addr, uint16_t v){ write_mem16_seg(seg, addr, v); };
		void put_data32(sgreg_t seg, uint32_t addr, uint32_t v){ write_mem32_seg(seg, addr, v); };

		uint8_t get_code8(int index){ return exec_mem8_seg(CS, get_eip() + index); };
		uint16_t get_code16(int index){ return exec_mem16_seg(CS, get_eip() + index); };
		uint32_t get_code32(int index){ return exec_mem32_seg(CS, get_eip() + index); };

		void push32(uint32_t v);
		uint32_t pop32(void);
		void push16(uint16_t v);
		uint16_t pop16(void);

	private:
		uint32_t trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr);
		uint32_t trans_v2l(acsmode_t mode, sgreg_t seg, uint32_t vaddr);

		bool search_tlb(uint32_t vpn, PTE *pte);
		void cache_tlb(uint32_t vpn, PTE pte);

		uint32_t read_mem32_seg(sgreg_t seg, uint32_t addr);
		uint16_t read_mem16_seg(sgreg_t seg, uint32_t addr);
		uint8_t read_mem8_seg(sgreg_t seg, uint32_t addr);
		void write_mem32_seg(sgreg_t seg, uint32_t addr, uint32_t v);
		void write_mem16_seg(sgreg_t seg, uint32_t addr, uint16_t v);
		void write_mem8_seg(sgreg_t seg, uint32_t addr, uint8_t v);
		uint32_t exec_mem32_seg(sgreg_t seg, uint32_t addr) { return read_mem32(trans_v2p(MODE_EXEC, seg, addr)); };
		uint16_t exec_mem16_seg(sgreg_t seg, uint32_t addr) { return read_mem16(trans_v2p(MODE_EXEC, seg, addr)); };
		uint8_t exec_mem8_seg(sgreg_t seg, uint32_t addr) { return read_mem8(trans_v2p(MODE_EXEC, seg, addr)); };
};

#endif
