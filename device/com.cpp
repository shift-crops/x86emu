#include <stdio.h>
#include "device/com.hpp"

uint8_t COM::in8(void){
	return getchar();
}

void COM::out8(uint8_t v){
	putchar(v);
}
