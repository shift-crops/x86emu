#ifndef _IO_H
#define _IO_H

#include <map>
#include "common.hpp" 
#include "memory.hpp" 
#include "device/dev_io.hpp" 

class IO {
	private:
		Memory *memory;
		std::map<uint16_t, PortIO*> port_io;
		std::map<uint16_t, size_t> port_io_map;
		std::map<uint32_t, MemoryIO*> mem_io;
		std::map<uint32_t, size_t> mem_io_map;

	public:
		IO(Memory *mem) { memory = mem; };
		~IO();
		void set_portio(uint16_t addr, size_t len, PortIO *dev) {
			addr &= ~1; port_io[addr] = dev; port_io_map[addr] = len; };
		void set_memio(uint32_t addr, size_t len, MemoryIO *dev) {
			addr &= ~1; mem_io[addr] = dev; mem_io_map[addr] = len; dev->set_mem(memory, addr, len);
		};

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
		uint16_t get_portio_base(uint16_t addr);
		uint32_t get_memio_base(uint32_t addr);
};

#endif
