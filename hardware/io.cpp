#include <stdint.h>
#include <unordered_map>
#include "hardware/io.hpp"

IO::~IO(){
	std::unordered_map<uint16_t, PortIO*>::iterator it_p;
	std::unordered_map<uint32_t, MemoryIO*>::iterator it_m;

//	for(it_p = port_io.begin(); it_p != port_io.end(); it_p++)
//		delete it_p->second;
	port_io.clear();

//	for(it_m = mem_io.begin(); it_m != mem_io.end(); it_m++)
//		delete it_m->second;
	mem_io.clear();
	mem_io_map.clear();
}

/* port IO */
void IO::set_portio(uint16_t addr, size_t len, PortIO *dev) {
	addr &= ~1;

	port_io[addr] = dev;
	port_io_map[addr] = len;
}

uint16_t IO::get_portio_base(uint16_t addr){
	for(int i=0; i<5; i++){	// max_mask : 0xfff0
		uint16_t base = (addr&(~1)) - (2*i);
		if(port_io_map.count(base))
			return addr < base+port_io_map[base] ? base : 0;
	}
	return 0;
}

uint32_t IO::in_io32(uint16_t addr){
	uint32_t v = 0;

	for(int i=0; i<4; i++)
		v += in_io8(addr+i) << (8*i);
	return v;
}

uint16_t IO::in_io16(uint16_t addr){
	uint16_t v = 0;

	for(int i=0; i<2; i++)
		v += in_io8(addr+i) << (8*i);
	return v;
}

uint8_t IO::in_io8(uint16_t addr){
	uint8_t v = 0;
	uint16_t base = get_portio_base(addr);
	if(base)
		v = port_io[base]->in8(addr);
	else
		ERROR("no device connected at port : 0x%04x", addr);
	INFO(4, "in [0x%04x] (0x%04x)", addr, v);
	return v;
}

void IO::out_io32(uint16_t addr, uint32_t value){
	for(int i=0; i<4; i++)
		out_io8(addr+i, (value >> (8*i)) & 0xff);
}

void IO::out_io16(uint16_t addr, uint16_t value){
	for(int i=0; i<2; i++)
		out_io8(addr+i, (value >> (8*i)) & 0xff);
}

void IO::out_io8(uint16_t addr, uint8_t value){
	uint16_t base = get_portio_base(addr);
	if(base)
		port_io[base]->out8(addr, value);
	else
		ERROR("no device connected at port : 0x%04x", addr);
	INFO(4, "out [0x%04x] (0x%04x)", addr, value);
}

/* memory mapped IO */
void IO::set_memio(uint32_t base, size_t len, MemoryIO *dev) {
	uint32_t addr;

	ASSERT(!(base&((1<<12)-1)));

	dev->set_mem(memory, base, len);
	mem_io[base] = dev;

	for(addr=base; addr<base+len; addr+=(1<<12))
		mem_io_map[addr] = base;
};

uint32_t IO::get_memio_base(uint32_t addr){
	addr &= (~((1<<12)-1));
	return mem_io_map.count(addr) ? mem_io_map[addr] : 0;
}

uint32_t IO::read_memio32(uint32_t base, uint32_t offset){
	ASSERT(mem_io.count(base));
	return mem_io[base]->read32(offset);
}

uint16_t IO::read_memio16(uint32_t base, uint32_t offset){
	ASSERT(mem_io.count(base));
	return mem_io[base]->read16(offset);
}

uint8_t IO::read_memio8(uint32_t base, uint32_t offset){
	ASSERT(mem_io.count(base));
	return mem_io[base]->read8(offset);
}

void IO::write_memio32(uint32_t base, uint32_t offset, uint32_t value){
	ASSERT(mem_io.count(base));
	mem_io[base]->write32(offset, value);
}

void IO::write_memio16(uint32_t base, uint32_t offset, uint16_t value){
	ASSERT(mem_io.count(base));
	mem_io[base]->write16(offset, value);
}

void IO::write_memio8(uint32_t base, uint32_t offset, uint8_t value){
	ASSERT(mem_io.count(base));
	mem_io[base]->write8(offset, value);
}
