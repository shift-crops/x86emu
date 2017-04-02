#include "device/syscontrol.hpp"

uint8_t SysControl::in8(uint16_t addr){
	return mem->is_ena_a20gate() << 1;
}

void SysControl::out8(uint16_t addr, uint8_t v){
	mem->set_a20gate((v>>1)&1);
	INFO("set A20 gate : %d", mem->is_ena_a20gate());
}
