#include <stdint.h>
#include <string.h>
#include "hardware/memory.hpp"

Memory::Memory(size_t size){
	mem_size = size;
	memory = new uint8_t[size];
	a20_gate = false;
}

Memory::~Memory(){
	delete[] memory;
	mem_size = 0;
}

void Memory::dump_mem(uint32_t addr, size_t size){
	addr &= ~(0x10-1);

	for(uint32_t idx=0; idx*0x10<size; idx++){
		MSG("0x%08x : ", addr+idx*0x10);
		for(uint8_t i=0; i<4; i++)
			MSG("%08x ", ((uint32_t*)memory)[(addr+idx*0x10)/4+i]);
		MSG("\n");
	}
}

size_t Memory::read_data(void *dst, uint32_t src_addr, size_t size){
	CHECK_RANGE(src_addr, size);

	memcpy(dst, &memory[src_addr], size);
	return size;
}

size_t Memory::write_data(uint32_t dst_addr, void *src, size_t size){
	CHECK_RANGE(dst_addr, size);

	memcpy(&memory[dst_addr], src, size);
	return size;
}
