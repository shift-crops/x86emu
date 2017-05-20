#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#define TYPE_TSS	0x01
#define TYPE_LDT	0x02
#define TYPE_CALL	0x04
#define TYPE_TASK	0x05
#define TYPE_INTERRUPT	0x06
#define TYPE_TRAP	0x07

#define TYPE_DATA	0x10
#define TYPE_CODE	0x18

struct Descriptor {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t type : 3;
	uint8_t D : 1;
	uint8_t S : 1;
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint8_t limit_h : 4;
	uint8_t AVL : 1;
	uint8_t : 2;
	uint8_t G : 1;
	uint8_t base_h;
};

// Segment Descriptor
struct SegDesc {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	union {
		union {
			struct {
				uint8_t : 1;
				uint8_t w : 1;		// 0:r, 1:rw
				uint8_t exd: 1;
				uint8_t : 5;
			} data;
			struct {
				uint8_t : 1;
				uint8_t r : 1;		// 0:x, 1;xr
				uint8_t cnf : 1;
				uint8_t : 5;
			} code;
			struct{
				uint8_t A : 1;
				uint8_t : 2;
				uint8_t segc : 1;
				uint8_t : 4;
			};
		} type;

		struct {
			uint8_t : 4;
			uint8_t S : 1;			// 1
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

// System Segment Descriptor
struct TSSDesc {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	union {
		struct {
			uint8_t : 1;
			uint8_t B : 1;
		};

		struct {
			uint8_t type : 3;	// 1 or 3
			uint8_t D : 1;		// 0:16bit, 1:32bit
			uint8_t S : 1;		// 0
			uint8_t DPL : 2;
			uint8_t P : 1;
		};
	};
	uint8_t limit_h : 4;
	uint8_t AVL : 1;
	uint8_t : 2;
	uint8_t G : 1;
	uint8_t base_h;
};

struct LDTDesc {
	uint16_t limit_l;
	uint16_t base_l;
	uint8_t base_m;
	uint8_t type : 3;	// 2
	uint8_t D : 1;
	uint8_t S : 1;		// 0
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint8_t limit_h : 4;
	uint8_t AVL : 1;
	uint8_t : 2;
	uint8_t G : 1;
	uint8_t base_h;
};

// Gate Descriptor
struct CallGateDesc {
	uint16_t offset_l;
	uint16_t seg_sel;
	uint8_t pc : 5;
	uint8_t : 3;
	uint8_t type : 3;	// 4
	uint8_t D : 1;
	uint8_t S : 1;		// 0
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint16_t offset_h;
};

struct TaskGateDesc {
	uint16_t : 16;
	uint16_t tss_sel;
	uint8_t : 8;
	uint8_t type : 3;	// 5
	uint8_t : 1;
	uint8_t S : 1;		// 0
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint16_t : 16;
};

struct IntGateDesc {
	uint16_t offset_l;
	uint16_t seg_sel;
	uint8_t : 8;
	uint8_t type : 3;	// 6
	uint8_t D : 1;
	uint8_t S : 1;		// 0
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint16_t offset_h;
};

struct TrapGateDesc {
	uint16_t offset_l;
	uint16_t seg_sel;
	uint8_t : 8;
	uint8_t type : 3;	// 7
	uint8_t D : 1;
	uint8_t S : 1;		// 0
	uint8_t DPL : 2;
	uint8_t P : 1;
	uint16_t offset_h;
};

struct TSS {
	uint16_t prev_sel;
	uint16_t : 16;
	uint32_t esp0;
	uint16_t ss0;
	uint16_t : 16;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t : 16;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t : 16;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t : 16;
	uint16_t cs;
	uint16_t : 16;
	uint16_t ss;
	uint16_t : 16;
	uint16_t ds;
	uint16_t : 16;
	uint16_t fs;
	uint16_t : 16;
	uint16_t gs;
	uint16_t : 16;
	uint16_t ldtr;
	uint16_t : 16;
	uint16_t T : 1;
	uint16_t : 15;
	uint16_t io_base;
};

#endif
