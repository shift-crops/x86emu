#include <map>
#include <vector>
#include "emulator/access.hpp"
#include "emulator/exception.hpp"
#include "emulator/descriptor.hpp"
#include "hardware/processor.hpp"
#include "hardware/cr.hpp"

void DataAccess::set_segment(sgreg_t reg, uint16_t v){
	SGRegister sg;
	SGRegCache *cache = &sg.cache;

	get_sgreg(reg, &sg);
	sg.raw = v;

	if(is_protected()){
		uint32_t dt_base;
		uint16_t dt_limit, dt_index;
		SegDesc gdt;
		const char* sgreg_name[] = { "ES", "CS", "SS", "DS", "FS", "GS" };

		dt_index = sg.index << 3;

		dt_base = get_dtreg_base(sg.TI ? LDTR : GDTR);
		dt_limit = get_dtreg_limit(sg.TI ? LDTR : GDTR);

		EXCEPTION(EXP_GP, (reg == CS || reg == SS) && !dt_index);
		EXCEPTION(EXP_GP, dt_index > dt_limit);

		read_data(&gdt, dt_base + dt_index, sizeof(SegDesc));

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

uint8_t DataAccess::type_descriptor(uint16_t sel){
	uint32_t gdt_base;
	uint16_t gdt_limit;
	Descriptor desc;

	gdt_base = get_dtreg_base(GDTR);
	gdt_limit = get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	read_data(&desc, gdt_base + sel, sizeof(Descriptor));
	if(desc.S){
		if(((SegDesc*)&desc)->type.segc)
			return TYPE_CODE;
		else
			return TYPE_DATA;
	}
	else if(desc.type == 3)
		return TYPE_TSS;

	return desc.type;
}

void DataAccess::set_ldtr(uint16_t sel){
	uint32_t gdt_base, base;
	uint16_t gdt_limit, limit;
	LDTDesc ldt;

	gdt_base = get_dtreg_base(GDTR);
	gdt_limit = get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	read_data(&ldt, gdt_base + sel, sizeof(LDTDesc));

	base = (ldt.base_h << 24) + (ldt.base_m << 16) + ldt.base_l;
	limit = (ldt.limit_h << 16) + ldt.limit_l;
	set_dtreg(LDTR, sel, base, limit); 
}

void DataAccess::set_tr(uint16_t sel){
	uint32_t gdt_base, base;
	uint16_t gdt_limit, limit;
	TSSDesc tssdesc;

	gdt_base = get_dtreg_base(GDTR);
	gdt_limit = get_dtreg_limit(GDTR);
	EXCEPTION(EXP_GP, sel > gdt_limit);

	read_data(&tssdesc, gdt_base + sel, sizeof(TSSDesc));
	EXCEPTION(EXP_GP, tssdesc.type != TYPE_TSS);

	base = (tssdesc.base_h << 24) + (tssdesc.base_m << 16) + tssdesc.base_l;
	limit = (tssdesc.limit_h << 16) + tssdesc.limit_l;

	set_dtreg(TR, sel, base, limit); 
}

void DataAccess::switch_task(uint16_t sel){
	uint32_t base;
	uint16_t prev, limit;
	TSS old_tss, new_tss;  

	prev = get_dtreg_selector(TR);
	base = get_dtreg_base(TR);
	limit = get_dtreg_limit(TR);
	EXCEPTION(EXP_GP, limit < sizeof(TSS)-1);

	read_data(&old_tss, base, sizeof(TSS));
	old_tss.cr3 = get_crn(3);
	old_tss.eip = get_eip();
	old_tss.eflags = get_eflags();
	old_tss.eax = get_gpreg(EAX);
	old_tss.ecx = get_gpreg(ECX);
	old_tss.edx = get_gpreg(EDX);
	old_tss.ebx = get_gpreg(EBX);
	old_tss.esp = get_gpreg(ESP);
	old_tss.ebp = get_gpreg(EBP);
	old_tss.esi = get_gpreg(ESI);
	old_tss.edi = get_gpreg(EDI);
	old_tss.es = get_segment(ES);
	old_tss.cs = get_segment(CS);
	old_tss.ss = get_segment(SS);
	old_tss.ds = get_segment(DS);
	old_tss.fs = get_segment(FS);
	old_tss.gs = get_segment(GS);
	write_data(base, &old_tss, sizeof(TSS));

	set_tr(sel);

	base = get_dtreg_base(TR);
	limit = get_dtreg_limit(TR);
	EXCEPTION(EXP_GP, limit < sizeof(TSS)-1);

	read_data(&new_tss, base, sizeof(TSS));
	new_tss.prev_sel = prev;
	write_data(base, &new_tss, sizeof(TSS));
	set_crn(3, new_tss.cr3);
	set_eip(new_tss.eip);
	set_eflags(new_tss.eflags);
	set_gpreg(EAX, new_tss.eax);
	set_gpreg(ECX, new_tss.ecx);
	set_gpreg(EDX, new_tss.edx);
	set_gpreg(EBX, new_tss.ebx);
	set_gpreg(ESP, new_tss.esp);
	set_gpreg(EBP, new_tss.ebp);
	set_gpreg(ESI, new_tss.esi);
	set_gpreg(EDI, new_tss.edi);
	set_segment(ES, new_tss.es);
	set_segment(CS, new_tss.cs);
	set_segment(SS, new_tss.ss);
	set_segment(DS, new_tss.ds);
	set_segment(FS, new_tss.fs);
	set_segment(GS, new_tss.gs);
}

void DataAccess::jmpf(uint16_t sel, uint32_t eip){
	if(is_protected()){
		switch(type_descriptor(sel)){
			case TYPE_CODE:
				goto jmp;
			case TYPE_TSS:
				switch_task(sel);
				return;
		}
	}

jmp:
	INFO(2, "cs = 0x%04x, eip = 0x%08x", sel, eip);
	set_segment(CS, sel);
	set_eip(eip);
}

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

