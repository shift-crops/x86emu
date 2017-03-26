#ifndef _IO_H
#define _IO_H

#include <stdint.h>
#include <map>
#include "common.hpp" 
#include "access.hpp" 
#include "device/device_io.hpp" 

class IO : private virtual DataAccess {
	private:
		std::map<uint16_t, DeviceIO*> port_io;
		std::map<uint32_t, DeviceIO*> mem_io;
	public:
		IO();
		~IO();
		uint32_t in_io32(uint16_t addr);
		uint16_t in_io16(uint16_t addr);
		uint8_t in_io8(uint16_t addr);
		void out_io32(uint16_t addr, uint32_t v);
		void out_io16(uint16_t addr, uint16_t v);
		void out_io8(uint16_t addr, uint8_t v);
};

#endif
