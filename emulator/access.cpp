#include "emulator/access.hpp"
#include "hardware/processor.hpp"
#include "hardware/cr.hpp"

DataAccess::DataAccess(){
}

uint32_t DataAccess::trans_v2p_seg(segreg_t seg, uint32_t vaddr){
	uint32_t paddr;

	if(is_protected()){
	}
	else{
		paddr = (get_segreg(seg) << 4) + vaddr;
		paddr &= (1<<20)-1;			// A20 mask
	}

	return paddr;
}

void DataAccess::push32(uint32_t value){
	uint32_t esp;

	update_gpreg(ESP, -4);
	esp = get_gpreg(ESP);
	write_mem32_seg(SS, esp, value);
}

uint32_t DataAccess::pop32(void){
	uint32_t esp, value;

	esp = get_gpreg(ESP);
	value = read_mem32_seg(SS, esp);
	update_gpreg(ESP, 4);

	return value;
}

void DataAccess::push16(uint16_t value){
	uint16_t sp;

	update_gpreg(SP, -2);
	sp = get_gpreg(SP);
	write_mem16_seg(SS, sp, value);
}

uint16_t DataAccess::pop16(void){
	uint16_t sp, value;

	sp = get_gpreg(SP);
	value = read_mem16_seg(SS, sp);
	update_gpreg(SP, 2);

	return value;
}

