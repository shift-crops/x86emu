#ifndef _DATA_ACCESS_H
#define _DATA_ACCESS_H

#include "common.hpp"
#include <vector>
#include "hardware/hardware.hpp"
//#include "util/lru.hpp"

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

struct TSS {
	uint16_t prev_sel;
	uint16_t : 16;
	uint32_t esp0;
	uint16_t ss0;
	uint16_t : 16;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t : 16;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t : 16;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t : 16;
	uint16_t cs;
	uint16_t : 16;
	uint16_t ss;
	uint16_t : 16;
	uint16_t ds;
	uint16_t : 16;
	uint16_t fs;
	uint16_t : 16;
	uint16_t gs;
	uint16_t : 16;
	uint16_t ldtr;
	uint16_t : 16;
	uint16_t T : 1;
	uint16_t : 15;
	uint16_t io_base;
};


enum acsmode_t { MODE_READ, MODE_WRITE, MODE_EXEC };

class DataAccess : public virtual Hardware {
	private:
		//LRU<uint32_t, PTE> *tlb;
		std::vector<PTE*> tlb;

	public:
		//DataAccess(){ tlb = new LRU<uint32_t, PTE>(128); }
		void set_segment(sgreg_t seg, uint16_t v);
		uint16_t get_segment(sgreg_t seg);

		void set_gdtr(uint32_t base, uint16_t limit){ set_dtreg(GDTR, 0, base, limit); };
		void set_idtr(uint32_t base, uint16_t limit){ set_dtreg(IDTR, 0, base, limit); };

		void set_ldtr(uint16_t sel);
		uint16_t get_ldtr(void) { return get_dtreg_selector(LDTR); };
		void set_tr(uint16_t sel);
		uint16_t get_tr(void) { return get_dtreg_selector(TR); };

		uint8_t type_descriptor(uint16_t sel);
		void switch_task(uint16_t sel);
		void jmpf(uint16_t sel, uint32_t eip);

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
