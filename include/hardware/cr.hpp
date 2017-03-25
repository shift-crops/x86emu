#ifndef _CR_H
#define _CR_H

#include <stdint.h>
#include "common.hpp"

class CR {
	private:
		union {
			uint32_t raw;
			struct {
				uint32_t PE : 1;
				uint32_t MP : 1;
				uint32_t EM : 1;
				uint32_t TS : 1;
				uint32_t ET : 1;
				uint32_t NE : 1;
				uint32_t : 10;
				uint32_t WP : 1;
				uint32_t : 1;
				uint32_t AM : 1;
				uint32_t : 10;
				uint32_t NW : 1;
				uint32_t CD : 1;
				uint32_t PG : 1;
			};
		} cr0;

		union {
			uint32_t raw;
		} cr1;

		union {
			uint32_t raw;
		} cr2;

		union {
			uint32_t raw;
			struct {
				uint32_t : 3;
				uint32_t PWT : 1;
				uint32_t PCD : 1;
				uint32_t : 7;
				uint32_t PageDirBase : 20;
			};
		} cr3;

		union {
			uint32_t raw;
			struct {
				uint32_t VME : 1;
				uint32_t PVI : 1;
				uint32_t TSD : 1;
				uint32_t DE : 1;
				uint32_t PSE: 1;
				uint32_t PAE: 1;
				uint32_t MCE: 1;
				uint32_t PGE: 1;
				uint32_t PCE: 1;
				uint32_t OSFXSR: 1;
				uint32_t OSXMMEXCPT: 1;
				uint32_t : 21;
			};
		} cr4;

	public:
		CR() { set_cr0(0); set_cr1(0); set_cr2(0); set_cr3(0); set_cr4(0);};

		uint32_t get_cr0(void) { return cr0.raw; };
		uint32_t get_cr1(void) { return cr1.raw; };
		uint32_t get_cr2(void) { return cr2.raw; };
		uint32_t get_cr3(void) { return cr3.raw; };
		uint32_t get_cr4(void) { return cr4.raw; };

		void set_cr0(uint32_t v) { cr0.raw = v; };
		void set_cr1(uint32_t v) { cr1.raw = v; };
		void set_cr2(uint32_t v) { cr2.raw = v; };
		void set_cr3(uint32_t v) { cr3.raw = v; };
		void set_cr4(uint32_t v) { cr4.raw = v; };

		bool is_protected(void) { return cr0.PE; };
};

#endif
