#include "device/keyboard.hpp"

uint8_t Keyboard::in8(uint16_t addr){
	uint8_t c = code;
	code = 0;

	return c;
}

void Keyboard::out8(uint16_t addr, uint8_t v){
}

bool Keyboard::send_code(uint32_t scancode){
	if(code)
		return false;

	code = scancode;
	intr = true;
	return true;
}
