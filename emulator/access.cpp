#include "emulator/access.hpp"
#include "hardware/processor.hpp"
#include "hardware/cr.hpp"

DataAccess::DataAccess(){
}

uint32_t DataAccess::trans_v2p(sgreg_t seg, uint32_t vaddr){
	uint32_t laddr, paddr;

	laddr = trans_v2l(seg, vaddr);

//	if(is_protected()){
//	}
//	else
		paddr = laddr;

	if(chk_a20gate())
		paddr &= (1<<20)-1;

	return paddr;
}

uint32_t DataAccess::trans_v2l(sgreg_t seg, uint32_t vaddr){
	uint32_t laddr;

	if(is_protected()){
		uint32_t gdt_base, base;
		uint16_t gdt_limit, gdt_offset, limit;
		DescriptorTable gdt;

		gdt_base = get_dtreg_base(GDTR);
		gdt_limit = get_dtreg_limit(GDTR);
		gdt_offset = get_sgreg(seg);

		//INFO("gdt_base=0x%04x, gdt_limit=0x%02x, gdt_offset=0x%02x", gdt_base, gdt_limit, gdt_offset);
		if(gdt_offset > gdt_limit)
			throw "gdt_limit";

		read_data(&gdt, gdt_base + gdt_offset, sizeof(DescriptorTable));

		base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
		limit = (gdt.limit_h << 16) + gdt.limit_l;
		//INFO("base=0x%04x, limit=0x%02x, vaddr=0x%02x", base, limit, vaddr);
		if(vaddr > limit)
			throw "segmentation_limit";

		laddr = base + vaddr;
		//INFO("gdtr_base=0x%04x, offset=0x%02x, base=0x%04x, limit=0x%04x", gdtr_base, get_sgreg(seg), base, limit);
	}

	else
		laddr = (get_sgreg(seg) << 4) + vaddr;

	return laddr;
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

