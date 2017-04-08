#include "device/mouse.hpp"

uint8_t Mouse::in8(uint16_t addr){
	return 0;
}

void Mouse::out8(uint16_t addr, uint8_t v){
}

void Mouse::test(void){
	intr = true;
}
