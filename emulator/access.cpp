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
		EXCEPTION(EXP_GP, !dt_index || dt_index > dt_limit);

		read_data(&gdt, dt_base + dt_index, sizeof(SGDescriptor));

		base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
		limit = (gdt.limit_h << 16) + gdt.limit_l;
		if(gdt.G) limit <<= 12;

		if(gdt.type.segc){
			EXCEPTION(EXP_GP, mode == MODE_WRITE || (mode == MODE_READ && !gdt.type.code.r));
			EXCEPTION(EXP_GP, sg.RPL > gdt.DPL && !(mode == MODE_EXEC && gdt.type.code.cnf));
		}
		else{
			EXCEPTION(EXP_GP, mode == MODE_EXEC || (mode == MODE_WRITE && !gdt.type.data.w));
			EXCEPTION(EXP_GP, sg.RPL > gdt.DPL);

			if(gdt.type.data.exd)
				base -= limit;
		}
		EXCEPTION(EXP_GP, vaddr > limit);

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

uint32_t DataAccess::read_mem32_seg(sgreg_t seg, uint32_t addr){
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return chk_memio(paddr) ? read_memio32(paddr) : read_mem32(paddr);
}

uint16_t DataAccess::read_mem16_seg(sgreg_t seg, uint32_t addr) {
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return chk_memio(paddr) ? read_memio16(paddr) : read_mem16(paddr);
}

uint8_t DataAccess::read_mem8_seg(sgreg_t seg, uint32_t addr){
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return chk_memio(paddr) ? read_memio8(paddr) : read_mem8(paddr);
}

void DataAccess::write_mem32_seg(sgreg_t seg, uint32_t addr, uint32_t v){
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if(chk_memio(paddr))
		write_memio32(paddr, v);
	else
		write_mem32(paddr, v);
}

void DataAccess::write_mem16_seg(sgreg_t seg, uint32_t addr, uint16_t v){
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	INFO("Write : 0x%04x(0x%04x)", paddr, v);
	if(chk_memio(paddr))
		write_memio16(paddr, v);
	else
		write_mem16(paddr, v);
}

void DataAccess::write_mem8_seg(sgreg_t seg, uint32_t addr, uint8_t v){
	uint32_t paddr;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if(chk_memio(paddr))
		write_memio8(paddr, v);
	else
		write_mem8(paddr, v);
}

