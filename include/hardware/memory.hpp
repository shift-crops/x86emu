#ifndef _MEMORY_H
#define _MEMORY_H

#include "common.hpp" 

#define DEFAULT_MEMORY_SIZE 	(1*KB)
#define ASSERT_RANGE(addr, len)	ASSERT(addr+len-1 < mem_size)
#define IN_RANGE(addr, len)	(addr+len-1 < mem_size)

class Memory {
	private:
		uint32_t mem_size;
		uint8_t *memory;
		bool a20gate;

	public:
		Memory(size_t size = DEFAULT_MEMORY_SIZE);
		~Memory();

		void dump_mem(uint32_t addr, size_t size);

		size_t read_data(void *dst, uint32_t src_addr, size_t size);
		size_t write_data(uint32_t dst_addr, void *src, size_t size);

		uint32_t read_mem32(uint32_t addr){ return IN_RANGE(addr, 4) ? *((uint32_t*)&memory[addr]) : 0; };
		uint16_t read_mem16(uint32_t addr){ return IN_RANGE(addr, 2) ? *((uint16_t*)&memory[addr]) : 0; };
		uint8_t read_mem8(uint32_t addr){ return IN_RANGE(addr, 1) ? memory[addr] : 0; };

		void write_mem32(uint32_t addr, uint32_t v){ if(IN_RANGE(addr, 4)) *((uint32_t*)&memory[addr]) = v; };
		void write_mem16(uint32_t addr, uint16_t v){ if(IN_RANGE(addr, 2)) *((uint16_t*)&memory[addr]) = v; };
		void write_mem8(uint32_t addr, uint8_t v){ if(IN_RANGE(addr, 1)) memory[addr] = v; };

		bool is_ena_a20gate(void) { return a20gate; };
		void set_a20gate(bool ena) { a20gate = ena; };
};

#endif
