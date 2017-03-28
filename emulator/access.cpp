#include "emulator/access.hpp"
#include "emulator/exception.hpp"
#include "emulator/descriptor.hpp"
#include "hardware/processor.hpp"
#include "hardware/cr.hpp"

uint32_t DataAccess::trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr){
	uint32_t laddr, paddr;

	laddr = trans_v2l(mode, seg, vaddr);

//	if(is_protected()){
//	}
//	else
		paddr = laddr;

	if(chk_a20gate())
		paddr &= (1<<20)-1;

	return paddr;
}

uint32_t DataAccess::trans_v2l(acsmode_t mode, sgreg_t seg, uint32_t vaddr){
	uint32_t laddr;

	if(is_protected()){
		uint32_t dt_base, base, limit;
		uint16_t dt_limit, dt_index;
		SGRegister sg;
		SGDescriptor gdt;

		*((uint16_t*)&sg) = get_sgreg(seg);
		dt_index = sg.index << 3;

		dt_base = get_dtreg_base(sg.TI ? LDTR : GDTR);
		dt_limit = get_dtreg_limit(sg.TI ? LDTR : GDTR);


		//INFO("dt_base=0x%04x, dt_limit=0x%02x, dt_index=0x%02x", dt_base, dt_limit, dt_index);
		if(!dt_index || dt_index > dt_limit)
			throw EXP_GP;

		read_data(&gdt, dt_base + dt_index, sizeof(SGDescriptor));

		base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
		limit = (gdt.limit_h << 16) + gdt.limit_l;
		if(gdt.G) limit <<= 12;

		if(gdt.type.segc){
			if(mode == MODE_WRITE || (mode == MODE_READ && !gdt.type.code.r))
				throw EXP_GP;
			if(sg.RPL > gdt.DPL && !(mode == MODE_EXEC && gdt.type.code.cnf))
				throw EXP_GP;
		}
		else{
			if(mode == MODE_EXEC || (mode == MODE_WRITE && !gdt.type.data.w))
				throw EXP_GP;
			if(sg.RPL > gdt.DPL)
				throw EXP_GP;
			if(gdt.type.data.exd)
				base -= limit;
		}

		if(vaddr > limit)
			throw EXP_GP;
		laddr = base + vaddr;
		//INFO("base=0x%04x, limit=0x%02x, laddr=0x%02x", base, limit, laddr);
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

