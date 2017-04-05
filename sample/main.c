#define uint32_t unsigned long

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

int _puts(char *);

int main(void){
	//char s[10];
	//_gets(s);
	while(1){
		_puts("puts in main function");
		//for(int i=0; i<1000; i++);
		__asm__("hlt");
	}
}

uint32_t init_paging(void){
	struct PDE *pde = (struct PDE*)0x10000;
	struct PTE *pte = (struct PTE*)0x11000;
	uint32_t offset = 0x10000;

	pde[0].ptbl_base = ((uint32_t)pte+offset)>>12;
	pde[0].P = 1;
	pde[0].RW = 1;
	pde[0].US = 1;
	for(int i=0; i<20; i++){
		pte[i].page_base = i;
		pte[i].P = 1;
		pte[i].RW = 1;
		pte[i].US = 1;
	}

	return (uint32_t)pde+offset;
}
