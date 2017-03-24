#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <stdint.h>
#include "common.hpp"
#include "hardware/hardware.hpp"

class Emulator : public Hardware {
	public:
                Emulator(size_t size, uint32_t eip, uint32_t esp) : Hardware(size) { set_eip(eip); set_gpreg(ESP, esp); };

                void load_binary(const char* fname, uint32_t addr, size_t size);

                uint8_t get_code8(int index){ return read_mem8(get_eip() + index); };
                uint16_t get_code16(int index){ return read_mem16(get_eip() + index); };
                uint32_t get_code32(int index){ return read_mem32(get_eip() + index); };

                template <class T> uint32_t update_eflags_add(T v1, uint32_t v2) { return _eflags.update_add(v1, v2); };
                template <class T> uint32_t update_eflags_or(T v1, uint32_t v2) { return _eflags.update_or(v1, v2); };
                template <class T> uint32_t update_eflags_and(T v1, uint32_t v2) { return _eflags.update_and(v1, v2); };
                template <class T> uint32_t update_eflags_sub(T v1, uint32_t v2) { return _eflags.update_sub(v1, v2); };
                bool is_eflags_carry(void) { return _eflags.is_carry(); };
                bool is_eflags_parity(void) { return _eflags.is_parity(); };
                bool is_eflags_zero(void) { return _eflags.is_zero(); };
                bool is_eflags_sign(void) { return _eflags.is_sign(); };
                bool is_eflags_overflow(void) { return _eflags.is_overflow(); };

                //uint32_t read_mem32(uint32_t addr){ return read_mem32(addr); };
                //uint8_t read_mem8(uint32_t addr){ return read_mem8(addr); };
                //void write_mem32(uint32_t addr, uint32_t v){ write_mem32(addr, v); };
                //void write_mem8(uint32_t addr, uint8_t v){ write_mem8(addr, v); };
};

#endif
