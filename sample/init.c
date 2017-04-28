#include "common.h"

struct DTReg {
	uint16_t limit;
	uint16_t base_l;
	uint16_t base_h;
};

struct IDT {
        uint16_t offset_l;
        uint16_t selector;
        uint8_t : 8;
        uint8_t type : 3;
        uint8_t D : 1;
        uint8_t : 1;
        uint8_t DPL : 2;
        uint8_t P : 1;
        uint16_t offset_h;
};

struct PDE {
        uint32_t P : 1;
        uint32_t RW : 1;
        uint32_t US : 1;
        uint32_t PWT : 1;
        uint32_t PCD : 1;
        uint32_t A : 1;
        uint32_t : 1;
        uint32_t PS : 1;
        uint32_t G : 1;
        uint32_t : 3;
        uint32_t ptbl_base : 20;
};

struct PTE {
        uint32_t P : 1;
        uint32_t RW : 1;
        uint32_t US : 1;
        uint32_t PWT : 1;
        uint32_t PCD : 1;
        uint32_t A : 1;
        uint32_t D : 1;
        uint32_t PAT : 1;
        uint32_t G : 1;
        uint32_t : 3;
        uint32_t page_base : 20;
};

void set_idt(struct IDT *idt, void (*off)(), uint8_t type, uint8_t DPL, uint16_t sel);
extern void sys_puts(void);
extern void sys_gets(void);
extern void irq_timer(void);
extern void irq_keyboard(void);
extern void irq_mouse(void);

uint32_t init_paging(void){
	struct PDE *pde = (struct PDE*)0x20000;
	struct PTE *pte = (struct PTE*)0x21000;

	pde[0].ptbl_base = ((uint32_t)pte)>>12;
	pde[0].P = 1;
	pde[0].RW = 1;
	pde[0].US = 1;
	for(int i=0; i<20; i++){
		pte[i].page_base = i;
		pte[i].P = 1;
		pte[i].RW = 1;
		pte[i].US = 1;
	}
	for(int i=0xa0; i<0xc0; i++){
		pte[i].page_base = i;
		pte[i].P = 1;
		pte[i].RW = 1;
		pte[i].US = 1;
	}

	return (uint32_t)pde;
}

uint32_t init_idt(void){
	struct DTReg *idtr = (struct DTReg*)0x28000;
	struct IDT *idt = (struct IDT*)0x28030;

	idtr->limit = sizeof(struct IDT)*255 - 1;
	idtr->base_l = (uint32_t)idt & 0xffff;
	idtr->base_h = (uint32_t)idt >> 16;

	set_idt(&idt[0x00], sys_puts, 7, 3, 0x8); 
	set_idt(&idt[0x01], sys_gets, 7, 3, 0x8); 
	set_idt(&idt[0x20], irq_timer, 6, 0, 0x8); 
	set_idt(&idt[0x21], irq_keyboard, 6, 0, 0x8); 
	set_idt(&idt[0x2c], irq_mouse, 6, 0, 0x8); 

	return (uint32_t)idtr;
}

void set_idt(struct IDT *idt, void (*off)(), uint8_t type, uint8_t DPL, uint16_t sel){
	idt->offset_l = (uint32_t)off & 0xffff; 
	idt->offset_h = (uint32_t)off >> 16; 
	idt->selector = sel; 
	idt->type = type; 
	idt->D = 1; 
	idt->DPL = DPL; 
	idt->P = 1; 
}
