#include <stdint.h>
#include <map>
#include "hardware/io.hpp"

IO::~IO(){
	std::map<uint16_t, PortIO*>::iterator it_p;
	std::map<uint32_t, MemoryIO*>::iterator it_m;

	for(it_p = port_io.begin(); it_p != port_io.end(); it_p++)
		delete it_p->second;
	port_io.clear();

	for(it_m = mem_io.begin(); it_m != mem_io.end(); it_m++)
		delete it_m->second;
	mem_io.clear();
	mem_io_map.clear();
}

uint32_t IO::in_io32(uint16_t addr){
	uint32_t v = 0;
	for(int i=0; i<4; i++){
		uint16_t base_addr = (addr+i)&(~((1<<2)-1));
		if(port_io.count(base_addr))
			v += (port_io[base_addr]->in8(addr+i)) << (8*i);
		else
			ERROR("no device connected at port : 0x%04x", base_addr);
	}
	return v;
}

uint16_t IO::in_io16(uint16_t addr){
	uint16_t v = 0;
	for(int i=0; i<2; i++){
		uint16_t base_addr = (addr+i)&(~((1<<2)-1));
		if(port_io.count(base_addr))
			v += (port_io[base_addr]->in8(addr+i)) << (8*i);
		else
			ERROR("no device connected at port : 0x%04x", base_addr);
	}
	return v;
}

uint8_t IO::in_io8(uint16_t addr){
	uint8_t v = 0;
	uint16_t base_addr = addr&(~((1<<2)-1));
	if(port_io.count(base_addr))
		v = port_io[base_addr]->in8(addr);
	else
		ERROR("no device connected at port : 0x%04x", base_addr);
	return v;
}

void IO::out_io32(uint16_t addr, uint32_t value){
	for(int i=0; i<4; i++){
		uint16_t base_addr = (addr+i)&(~((1<<2)-1));
		if(port_io.count(base_addr))
			port_io[base_addr]->out8(addr+i, (value >> (8*i)) & 0xff);
		else
			ERROR("no device connected at port : 0x%04x", base_addr);
	}
}

void IO::out_io16(uint16_t addr, uint16_t value){
	for(int i=0; i<2; i++){
		uint16_t base_addr = (addr+i)&(~((1<<2)-1));
		if(port_io.count(base_addr))
			port_io[base_addr]->out8(addr+i, (value >> (8*i)) & 0xff);
		else
			ERROR("no device connected at port : 0x%04x", base_addr);
	}
}

void IO::out_io8(uint16_t addr, uint8_t value){
	uint16_t base_addr = addr&(~((1<<2)-1));
	if(port_io.count(base_addr))
		port_io[base_addr]->out8(addr, value);
	else
		ERROR("no device connected at port : 0x%04x", base_addr);
}


uint32_t IO::get_memio_base(uint32_t addr){
	std::map<uint32_t, size_t>::iterator it;

	for(it = mem_io_map.begin(); it != mem_io_map.end() && it->first <= addr; it++)
		if(addr >= it->first && addr < it->first+it->second)
			return it->first;
	return 0;
}

uint32_t IO::read_memio32(uint32_t addr){
	uint32_t v = 0;
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		v = mem_io[base]->read32(offset);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
	return v;
}

uint16_t IO::read_memio16(uint32_t addr){
	uint16_t v = 0;
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		v = mem_io[base]->read16(offset);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
	return v;
}

uint8_t IO::read_memio8(uint32_t addr){
	uint8_t v = 0;
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		v = mem_io[base]->read8(offset);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
	return v;
}

void IO::write_memio32(uint32_t addr, uint32_t value){
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		mem_io[base]->write32(offset, value);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
}

void IO::write_memio16(uint32_t addr, uint16_t value){
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		mem_io[base]->write16(offset, value);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
}

void IO::write_memio8(uint32_t addr, uint8_t value){
	uint32_t base, offset;

	base = get_memio_base(addr);
	if(base){
		offset = addr - base;
		mem_io[base]->write8(offset, value);
	}
	else
		ERROR("no device mapped : 0x%04x", addr);
}
