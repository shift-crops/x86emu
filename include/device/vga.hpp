#ifndef _VGA_H
#define _VGA_H

#include "common.hpp"
#include "dev_io.hpp"

#define r(reg)	(&reg.raw)

enum gmode_t {MODE_TEXT, MODE_GRAPHIC, MODE_GRAPHIC256};

class VGA : public PortIO, public MemoryIO {
	private:
		// General Register
		union {
			uint8_t raw;
			struct {
				uint8_t IO : 1;
				uint8_t ER : 1;
				uint8_t CLK0 : 1;
				uint8_t CLK1 : 1;
				uint8_t : 1;
				uint8_t PS : 1;
				uint8_t HSP : 1;
				uint8_t VSA : 1;
			};
		} mor;

		uint8_t *plane[4];
		bool refresh;

		class Sequencer : public PortIO {
			private:
				VGA *vga;

				union {
					uint8_t raw;
					struct {
						uint8_t INDX : 3;
					};
				} sar;

				union {
					uint8_t raw;
					struct {
						uint8_t _89DC : 1;
						uint8_t : 1;
						uint8_t SL : 1;
						uint8_t DC : 1;
						uint8_t S4 : 1;
						uint8_t SO : 1;
					};
				} cmr;

				union {
					uint8_t raw;
					struct {
						uint8_t MAP0E : 1;
						uint8_t MAP1E : 1;
						uint8_t MAP2E : 1;
						uint8_t MAP3E : 1;
					};
				} map_mr;

				union {
					uint8_t raw;
					struct {
						uint8_t CMB : 2;
						uint8_t CMA : 2;
						uint8_t CMBM : 1;
						uint8_t CMAM : 1;
					};
				} cmsr;

				union {
					uint8_t raw;
					struct {
						uint8_t : 1;
						uint8_t EM : 1;
						uint8_t OE : 1;
						uint8_t C4 : 1;
					};
				} mem_mr;

				uint8_t *regs[8] = {NULL, r(cmr), r(map_mr), r(cmsr), r(mem_mr), NULL, NULL, NULL};

			public:
				Sequencer(VGA *v) {
					vga = v;
					for(uint8_t i=0; i<sizeof(regs)/sizeof(uint8_t*); i++)
						if(regs[i])*regs[i]=0;
				};
				uint8_t *get_font(uint8_t att);
				uint8_t in8(uint16_t addr);
				void out8(uint16_t addr, uint8_t v);
		};

		class CRT : public PortIO {
			private:
				VGA *vga;

				union {
					uint8_t raw;
					struct {
						uint8_t INDX : 5;
					};
				} crtcar;

				union {
					uint8_t raw;
					uint8_t HT;
				} htr;

				union {
					uint8_t raw;
					uint8_t HDEE;
				} hdeer;

				union {
					uint8_t raw;
					uint8_t SHB;
				} shbr;

				union {
					uint8_t raw;
					struct {
						uint8_t EB : 5;
						uint8_t DESC : 2;
					};
				} ehbr;

				union {
					uint8_t raw;
					struct {
						uint8_t MSL : 5;
						uint8_t SVB9 : 1;
						uint8_t LC9 : 1;
						uint8_t LC : 1;
					};
				} mslr;

				union {
					uint8_t raw;
					struct {
						uint8_t RSCB : 5;
						uint8_t CO : 1;
					};
				} csr;

				union {
					uint8_t raw;
					struct {
						uint8_t RSCE : 5;
						uint8_t CSC : 2;
					};
				} cer;

				union {
					uint8_t raw;
					uint8_t HBSA;
				} sahr;

				union {
					uint8_t raw;
					uint8_t LBSA;
				} salr;

				union {
					uint8_t raw;
					uint8_t HBCL;
				} clhr;

				union {
					uint8_t raw;
					uint8_t LBCL;
				} cllr;

				union {
					uint8_t raw;
					uint8_t VDEE;
				} vdeer;

				union {
					uint8_t raw;
					uint8_t LLWS;
				} ofsr;

				union {
					uint8_t raw;
					struct {
						uint8_t CMS0 : 1;
						uint8_t SRSC : 1;
						uint8_t HRSX : 1;
						uint8_t C2 : 1;
						uint8_t : 1;
						uint8_t AW : 1;
						uint8_t WBM : 1;
						uint8_t HR : 1;
					};
				} crtmcr;

				uint8_t *regs[0x19] = {	r(htr), r(hdeer), r(shbr), r(ehbr), NULL, NULL, NULL, NULL,
							NULL, r(mslr), r(csr), r(cer), r(sahr), r(salr), r(clhr), r(cllr),
							NULL, NULL, r(vdeer), r(ofsr), NULL, NULL, NULL, r(crtmcr), NULL};

			public:
				CRT(VGA *v) {
					vga = v;
					for(uint8_t i=0; i<sizeof(regs)/sizeof(uint8_t*); i++)
						if(regs[i])*regs[i]=0;
				};
				void get_windowsize(uint16_t *x, uint16_t *y);
				uint8_t attr_index_text(uint32_t n);
				uint8_t in8(uint16_t addr);
				void out8(uint16_t addr, uint8_t v);
		};

		class GraphicController : public PortIO {
			private:
				VGA *vga;

				union {
					uint8_t raw;
					struct {
						uint8_t INDX : 4;
					};
				} gcar;

				union {
					uint8_t raw;
					struct {
						uint8_t SRM0 : 1;
						uint8_t SRM1 : 1;
						uint8_t SRM2 : 1;
						uint8_t SRM3 : 1;
					};
				} sr;

				union {
					uint8_t raw;
					struct {
						uint8_t ESRM0 : 1;
						uint8_t ESRM1 : 1;
						uint8_t ESRM2 : 1;
						uint8_t ESRM3 : 1;
					};
				} esr;

				union {
					uint8_t raw;
					struct {
						uint8_t CCM0 : 1;
						uint8_t CCM1 : 1;
						uint8_t CCM2 : 1;
						uint8_t CCM3 : 1;
					};
				} ccr;

				union {
					uint8_t raw;
					struct {
						uint8_t RC : 3;
						uint8_t FS : 2;
					};
				} drr;

				union {
					uint8_t raw;
					struct {
						uint8_t MS : 2;
					};
				} rmsr;

				union {
					uint8_t raw;
					struct {
						uint8_t WM : 2;
						uint8_t RM : 1;
						uint8_t OE : 1;
						uint8_t SRM : 1;
						uint8_t _256CM : 1;
					};
				} gmr;

				union {
					uint8_t raw;
					struct {
						uint8_t GM : 1;
						uint8_t OE : 1;
						uint8_t MM0 : 1;
						uint8_t MM1 : 1;
					};
				} mr;

				uint8_t *regs[9] = {r(sr), r(esr), r(ccr), r(drr), r(rmsr), r(gmr), r(mr), NULL, NULL};

			public:
				GraphicController(VGA *v) {
					vga = v;
					for(uint8_t i=0; i<sizeof(regs)/sizeof(uint8_t*); i++)
						if(regs[i])*regs[i]=0;
				};
				gmode_t graphic_mode(void);
				uint8_t attr_index_graphic(uint32_t n);
				uint8_t in8(uint16_t addr);
				void out8(uint16_t addr, uint8_t v);
		};

		class Attribute : public PortIO {
			private:
				VGA *vga;

				union {
					uint8_t raw;
					struct {
						uint8_t INDX : 5;
						uint8_t IPAS : 1;
					};
				} acar;

				union {
					uint8_t raw;
					struct {
						uint8_t P0 : 1;
						uint8_t P1 : 1;
						uint8_t P2 : 1;
						uint8_t P3 : 1;
						uint8_t P4 : 1;
						uint8_t P5 : 1;
					};
				} ipr[0x10];

				union {
					uint8_t raw;
					struct {
						uint8_t GAM : 1;
						uint8_t ME : 1;
						uint8_t ELGCC : 1;
						uint8_t ELSBI : 1;
						uint8_t : 1;
						uint8_t PELPC : 1;
						uint8_t PELW : 1;
						uint8_t P54S : 1;
					};
				} amcr;

				union {
					uint8_t raw;
					struct {
						uint8_t ECP : 4;
						uint8_t VSM : 2;
					};
				} cper;

				union {
					uint8_t raw;
					struct {
						uint8_t HPELP : 4;
					};
				} hpelpr;

				union {
					uint8_t raw;
					struct {
						uint8_t SC45 : 2;
						uint8_t SC67 : 2;
					};
				} csr;

				uint8_t *regs[0x15] = {	r(ipr[0x0]), r(ipr[0x1]), r(ipr[0x2]), r(ipr[0x3]),
							r(ipr[0x4]), r(ipr[0x5]), r(ipr[0x6]), r(ipr[0x7]), 
							r(ipr[0x8]), r(ipr[0x9]), r(ipr[0xa]), r(ipr[0xb]), 
							r(ipr[0xc]), r(ipr[0xd]), r(ipr[0xe]), r(ipr[0xf]), 
							r(amcr), NULL, r(cper), r(hpelpr), r(csr)};

			public:
				Attribute(VGA *v) {
					vga = v;
					for(uint8_t i=0; i<sizeof(regs)/sizeof(uint8_t*); i++)
						if(regs[i])*regs[i]=0;
				};
				uint8_t dac_index(uint8_t index);
				uint8_t in8(uint16_t addr);
				void out8(uint16_t addr, uint8_t v);
		};

		class DAC : public PortIO {
			private:
				VGA *vga;
				uint8_t progress;

				union {
					uint8_t raw[3];
					struct {
						uint8_t R : 6;
						uint8_t G : 6;
						uint8_t B : 6;
					};
				} clut[0x100];

				union {
					uint8_t raw;
					uint8_t index;
				} w_par;

				union {
					uint8_t raw;
					uint8_t index;
				} r_par;

				union {
					uint8_t raw;
					uint8_t color;
				} pdr;

				union {
					uint8_t raw;
					struct {
						uint8_t DACstate : 2;
					};
				} dacsr;

				union {
					uint8_t raw;
					uint8_t mask;
				} pelmr;

			public:
				DAC(VGA *v) { vga = v; };
				uint32_t translate_rgb(uint8_t index);
				uint8_t in8(uint16_t addr);
				void out8(uint16_t addr, uint8_t v);
		};

		Sequencer seq = Sequencer(this);
		CRT crt = CRT(this);
		GraphicController gc = GraphicController(this);
		Attribute attr = Attribute(this);
		DAC dac = DAC(this);

	public:
		VGA() { for(int i=0; i<4; i++) plane[i] = new uint8_t[1<<16]; };
		~VGA() { for(int i=0; i<4; i++) delete plane[i]; };
		bool need_refresh(void) { bool v = refresh; refresh = false; return v; };
		void get_windowsize(uint16_t *x, uint16_t *y);
		void rgb_image(uint8_t *buffer, uint32_t size);

		Sequencer *get_seq(void) { return &seq; };
		CRT *get_crt(void) { return &crt; };
		GraphicController *get_gc(void) { return &gc; };
		Attribute *get_attr(void) { return &attr; };
		DAC *get_dac(void) { return &dac; };

		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);

		uint8_t read8(uint32_t offset);
		void write32(uint32_t offset, uint32_t v);
		void write8(uint32_t offset, uint8_t v);
};

#endif
