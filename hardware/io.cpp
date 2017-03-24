#include <stdint.h>
#include "io.hpp"

IO::IO(){
}

uint32_t IO::in_io32(uint16_t addr){
	switch (addr){
		default:
			return 0;
	}
}

uint8_t IO::in_io8(uint16_t addr){
	switch (addr){
		case 0x03f8:
			return getchar();
			break;
		default:
			return 0;
	}
}

void IO::out_io32(uint16_t addr, uint32_t value){
	switch (addr) {
	}
}
void IO::out_io8(uint16_t addr, uint8_t value){
	switch (addr) {
		case 0x03f8:
			putchar(value);
			break;
	}
}
