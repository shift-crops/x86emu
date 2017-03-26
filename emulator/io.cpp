#include <stdint.h>
#include <map>
#include "emulator/io.hpp"
#include "device/devices.hpp"

IO::IO(){
	port_io[0x92] = new SysControl(this);
	port_io[0x3f8] = new COM();
}

IO::~IO(){
	std::map<uint16_t, DeviceIO*>::iterator it;
	for(it = port_io.begin(); it!= port_io.end(); it++)
		delete it->second;
	port_io.clear();
}

uint32_t IO::in_io32(uint16_t addr){
	uint32_t v = 0;
	for(int i=0; i<4; i++)
		if(port_io.count(addr+i))
			v += (port_io[addr+i]->in8()) << (8*i);
		else
			ERROR("no device connected at port : 0x%04x", addr+i);
	return v;
}

uint16_t IO::in_io16(uint16_t addr){
	uint16_t v = 0;
	for(int i=0; i<2; i++)
		if(port_io.count(addr+i))
			v += (port_io[addr+i]->in8()) << (8*i);
		else
			ERROR("no device connected at port : 0x%04x", addr+i);
	return v;
}

uint8_t IO::in_io8(uint16_t addr){
	uint8_t v = 0;
	if(port_io.count(addr))
		v = port_io[addr]->in8();
	else
		ERROR("no device connected at port : 0x%04x", addr);
	return v;
}

void IO::out_io32(uint16_t addr, uint32_t value){
	for(int i=0; i<4; i++)
		if(port_io.count(addr+i))
			port_io[addr+i]->out8((value >> (8*i)) & 0xff);
		else
			ERROR("no device connected at port : 0x%04x", addr+i);
}

void IO::out_io16(uint16_t addr, uint16_t value){
	for(int i=0; i<2; i++)
		if(port_io.count(addr+i))
			port_io[addr+i]->out8((value >> (8*i)) & 0xff);
		else
			ERROR("no device connected at port : 0x%04x", addr+i);
}

void IO::out_io8(uint16_t addr, uint8_t value){
	if(port_io.count(addr))
		port_io[addr]->out8(value);
	else
		ERROR("no device connected at port : 0x%04x", addr);
}
