#ifndef _STRUCTS_H
#define _STRUCTS_H

struct SGDescriptor {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	union {
		struct {
			union {
				struct {
					uint8_t : 1;
					uint8_t w : 1;
					uint8_t exd: 1;
					uint8_t : 5;
				} data;
				struct {
					uint8_t : 1;
					uint8_t r : 1;
					uint8_t cnf : 1;
					uint8_t : 5;
				} code;
				struct{
					uint8_t A : 1;
					uint8_t : 2;
					uint8_t segc : 1;
					uint8_t : 4;
				};
			};
		} type;

		struct {
			uint8_t : 4;
			uint8_t S : 1;
			uint8_t DPL : 2;
			uint8_t P : 1;
		};
	};
	uint8_t limit_h : 4;
	uint8_t AVL : 1;
	uint8_t : 1;
	uint8_t DB : 1;
	uint8_t G : 1;
	uint8_t base_h;
};


#define TYPE_TASK       5
#define TYPE_INTERRUPT  6
#define TYPE_TRAP       7

struct INTDescriptor {
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

struct TSSDescriptor {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t type : 4;
	uint8_t : 1;
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint8_t limit_h : 4;
	uint8_t AVL : 1;
	uint8_t : 2;
	uint8_t G : 1;
	uint8_t base_h;
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

#endif
