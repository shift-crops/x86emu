#define uint8_t	 unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long

typedef struct {
	uint16_t offset;
	uint16_t segment;
} IVT;

extern uint16_t bs_test;

void put_esmem(void* vaddr, uint16_t v);
void init_ivt(void);
void set_ivt(int n, uint32_t offset, uint16_t cs);

int bios_main(void){
	init_ivt();

	return 0;
}

void init_ivt(void){
	set_ivt(0x10, (uint32_t)(&bs_test), 0xf000);
}

void set_ivt(int n, uint32_t offset, uint16_t cs){
	IVT* ivt = (IVT*)0;

	put_esmem(&(ivt[n].offset), offset);
	put_esmem(&(ivt[n].segment), cs);
}
