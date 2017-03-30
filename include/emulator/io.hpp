#ifndef _IO_H
#define _IO_H

#include <stdint.h>
#include <map>
#include "common.hpp" 
#include "hardware/hardware.hpp" 
#include "device/device_io.hpp" 

class IO {
	private:
		Hardware *hardware;
		std::map<uint16_t, PortIO*> port_io;
		std::map<uint32_t, MemoryIO*> mem_io;
		std::map<uint32_t, size_t> mem_io_map;

	public:
		IO(Hardware *hw);
		~IO();
		uint32_t in_io32(uint16_t addr);
		uint16_t in_io16(uint16_t addr);
		uint8_t in_io8(uint16_t addr);
		void out_io32(uint16_t addr, uint32_t v);
		void out_io16(uint16_t addr, uint16_t v);
		void out_io8(uint16_t addr, uint8_t v);

		bool chk_memio(uint32_t addr) { return get_memio_base(addr); };
		uint32_t read_memio32(uint32_t addr);
		uint16_t read_memio16(uint32_t addr);
		uint8_t read_memio8(uint32_t addr);
		void write_memio32(uint32_t addr, uint32_t v);
		void write_memio16(uint32_t addr, uint16_t v);
		void write_memio8(uint32_t addr, uint8_t v);
	private:
		void set_portio(uint32_t addr, PortIO *dev) { port_io[addr] = dev; };
		void set_memio(uint32_t addr, size_t len, MemoryIO *dev) {
			mem_io[addr] = dev; mem_io_map[addr] = len; dev->set_mem(hardware, addr, len);
		};
		uint32_t get_memio_base(uint32_t addr);
};

#endif
