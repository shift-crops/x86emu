#include <map>
#include <vector>
#include "emulator/access.hpp"
#include "emulator/exception.hpp"
#include "emulator/structs.hpp"
#include "hardware/processor.hpp"
#include "hardware/cr.hpp"
//#include "util/lru.hpp"

uint32_t DataAccess::trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr){
	uint32_t laddr, paddr;

	laddr = trans_v2l(mode, seg, vaddr);

	if(is_ena_paging()){
		uint32_t vpn;
		uint16_t offset;
		uint8_t cpl;
		PTE pte;

		EXCEPTION(EXP_GP, !is_protected());

		cpl = get_segment(CS) & 3;
		vpn = laddr >> 12;
		offset = laddr & ((1<<12)-1);

		if(!search_tlb(vpn, &pte)){
			uint32_t pdir_base, ptbl_base;
			uint16_t pdir_index, ptbl_index;
			PDE pde;

			pdir_index = laddr >> 22;
			ptbl_index = (laddr >> 12) & ((1<<10)-1);

			pdir_base = get_pdir_base() << 12;
			read_data(&pde, pdir_base + pdir_index*sizeof(PDE), sizeof(PDE));
			EXCEPTION_WITH(EXP_PF, !pde.P, set_crn(2, laddr));
			EXCEPTION_WITH(EXP_PF, !pde.RW && mode == MODE_WRITE, set_crn(2, laddr));
			EXCEPTION_WITH(EXP_PF, !pde.US && cpl>2, set_crn(2, laddr));

			ptbl_base = pde.ptbl_base << 12;
			read_data(&pte, ptbl_base + ptbl_index*sizeof(PTE), sizeof(PTE));
			cache_tlb(vpn, pte);

			INFO(3, "Cache TLB : pdir_base=0x%04x, ptbl_base=0x%04x {vpn=0x%04x, pfn=0x%04x}"
					, pdir_base, ptbl_base, vpn, pte.page_base);
		}

		EXCEPTION_WITH(EXP_PF, !pte.P, set_crn(2, laddr));
		EXCEPTION_WITH(EXP_PF, !pte.RW && mode == MODE_WRITE, set_crn(2, laddr));
		EXCEPTION_WITH(EXP_PF, !pte.US && cpl>2, set_crn(2, laddr));

		paddr = (pte.page_base<<12) + offset;
	}
	else
		paddr = laddr;

	if(!is_ena_a20gate())
		paddr &= (1<<20)-1;

	return paddr;
}

bool DataAccess::search_tlb(uint32_t vpn, PTE *pte){
	if(vpn+1 > tlb.size() || !tlb[vpn])
		return false;

	ASSERT(pte);
	*pte = *tlb[vpn];

	return true;
}

void DataAccess::cache_tlb(uint32_t vpn, PTE pte){
	if(vpn+1 > tlb.size())
		tlb.resize(vpn+1, NULL);

	tlb[vpn] = new PTE;
	*tlb[vpn] = pte;
}

uint32_t DataAccess::trans_v2l(acsmode_t mode, sgreg_t seg, uint32_t vaddr){
	uint32_t laddr;
	SGRegister sg;

	get_sgreg(seg, &sg);

	if(is_protected()){
		uint32_t base, limit;
		SGRegCache cache = sg.cache;

		base = cache.base;
		limit = cache.limit;
		if(cache.flags.G) limit <<= 12;

		if(cache.flags.type.segc){
			EXCEPTION(EXP_GP, mode == MODE_WRITE);
			EXCEPTION(EXP_GP, mode == MODE_READ && !cache.flags.type.code.r);
			EXCEPTION(EXP_GP, sg.RPL > cache.flags.DPL && !(mode == MODE_EXEC && cache.flags.type.code.cnf));
		}
		else{
			EXCEPTION(EXP_GP, mode == MODE_EXEC);
			EXCEPTION(EXP_GP, mode == MODE_WRITE && !cache.flags.type.data.w);
			EXCEPTION(EXP_GP, sg.RPL > cache.flags.DPL);

			if(cache.flags.type.data.exd)
				base -= limit;
		}
		EXCEPTION(EXP_GP, vaddr > limit);

		laddr = base + vaddr;
		INFO(6, "base=0x%04x, limit=0x%02x, laddr=0x%02x", base, limit, laddr);
	}
	else
		laddr = (sg.raw << 4) + vaddr;

	return laddr;
}

void DataAccess::set_segment(sgreg_t reg, uint16_t v){
	SGRegister sg;
	SGRegCache *cache = &sg.cache;

	get_sgreg(reg, &sg);
	sg.raw = v;

	if(is_protected()){
		uint32_t dt_base;
		uint16_t dt_limit, dt_index;
		SGDescriptor gdt;
		const char* sgreg_name[] = { "ES", "CS", "SS", "DS", "FS", "GS" };

		dt_index = sg.index << 3;

		// TODO
		dt_base = get_dtreg_base(sg.TI ? LDTR : GDTR);
		dt_limit = get_dtreg_limit(sg.TI ? LDTR : GDTR);

		EXCEPTION(EXP_GP, (reg == CS || reg == SS) && !dt_index);
		EXCEPTION(EXP_GP, dt_index > dt_limit);

		read_data(&gdt, dt_base + dt_index, sizeof(SGDescriptor));

		cache->base = (gdt.base_h << 24) + (gdt.base_m << 16) + gdt.base_l;
		cache->limit = (gdt.limit_h << 16) + gdt.limit_l;

		*(uint8_t*)&cache->flags.type = *(uint8_t*)&gdt.type;
		cache->flags.AVL = gdt.AVL;
		cache->flags.DB = gdt.DB;
		cache->flags.G = gdt.G;

		INFO(3, "%s : dt_base=0x%04x, dt_limit=0x%02x, dt_index=0x%02x {base=0x%08x, limit=0x%08x, flags=0x%04x}"
				, sgreg_name[reg], dt_base, dt_limit, dt_index
				, cache->base, cache->limit<<(cache->flags.G ? 12 : 0), cache->flags.raw);

	}
	else
		cache->base = (uint32_t)v << 4;

	set_sgreg(reg, &sg);
}

inline uint16_t DataAccess::get_segment(sgreg_t reg){
	SGRegister sg;

	get_sgreg(reg, &sg);
	return sg.raw;
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
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio32(io_base, paddr-io_base) : read_mem32(paddr);
}

uint16_t DataAccess::read_mem16_seg(sgreg_t seg, uint32_t addr) {
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio16(io_base, paddr-io_base) : read_mem16(paddr);
}

uint8_t DataAccess::read_mem8_seg(sgreg_t seg, uint32_t addr){
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_READ, seg, addr);
	return (io_base = chk_memio(paddr)) ? read_memio8(io_base, paddr-io_base) : read_mem8(paddr);
}

void DataAccess::write_mem32_seg(sgreg_t seg, uint32_t addr, uint32_t v){
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if((io_base = chk_memio(paddr)))
		write_memio32(io_base, paddr-io_base, v);
	else
		write_mem32(paddr, v);
}

void DataAccess::write_mem16_seg(sgreg_t seg, uint32_t addr, uint16_t v){
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if((io_base = chk_memio(paddr)))
		write_memio16(io_base, paddr-io_base, v);
	else
		write_mem16(paddr, v);
}

void DataAccess::write_mem8_seg(sgreg_t seg, uint32_t addr, uint8_t v){
	uint32_t paddr, io_base;
       
	paddr = trans_v2p(MODE_WRITE, seg, addr);
	if((io_base = chk_memio(paddr)))
		write_memio8(io_base, paddr-io_base, v);
	else
		write_mem8(paddr, v);
}

