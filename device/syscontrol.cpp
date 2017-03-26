#include <stdio.h>
#include "device/syscontrol.hpp"

uint8_t SysControl::in8(void){
	return (!mem->chk_a20gate()) << 1;
}

void SysControl::out8(uint8_t v){
	mem->ena_a20gate(!((v>>1)&1));
	INFO("set A20 gate : %d", mem->chk_a20gate());
}
