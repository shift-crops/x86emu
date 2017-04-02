#include <stdio.h>
#include "device/com.hpp"

uint8_t COM::in8(uint16_t addr){
	return getchar();
}

void COM::out8(uint16_t addr, uint8_t v){
	putchar(v);
}
