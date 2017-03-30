#include <stdio.h>
#include "device/vram.hpp"

uint32_t VRAM::read32(uint32_t offset){
	uint32_t v;

	v = mem->read_mem32(paddr+offset);
	INFO("VRAM Read 32 (0x%04x) : 0x%04x", offset, v);
	return v;
	//return mem->read_mem32(paddr+offset);
}

uint16_t VRAM::read16(uint32_t offset){
	uint32_t v;

	v = mem->read_mem16(paddr+offset);
	INFO("VRAM Read 16 (0x%04x) : 0x%04x", offset, v);
	return v;
//	return mem->read_mem16(paddr+offset);
}

uint8_t VRAM::read8(uint32_t offset){
	uint32_t v;

	v = mem->read_mem8(paddr+offset);
	INFO("VRAM Read 8 (0x%04x) : 0x%04x", offset, v);
	return v;
//	return mem->read_mem8(paddr+offset);
}

void VRAM::write32(uint32_t offset, uint32_t v){
	INFO("VRAM Write 32 (0x%04x) : 0x%04x", offset, v);
	mem->write_mem32(paddr+offset, v);
}

void VRAM::write16(uint32_t offset, uint16_t v){
	INFO("VRAM Write 16 (0x%04x) : 0x%04x", offset, v);
	mem->write_mem16(paddr+offset, v);
}

void VRAM::write8(uint32_t offset, uint8_t v){
	INFO("VRAM Write 8 (0x%04x) : 0x%04x", offset, v);
	mem->write_mem8(paddr+offset, v);
}
