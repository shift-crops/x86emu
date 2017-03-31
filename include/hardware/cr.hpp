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

		uint32_t* cr[5];

	public:
		CR() {
			cr[0] = &cr0.raw;
			cr[1] = &cr1.raw;
			cr[2] = &cr2.raw;
			cr[3] = &cr3.raw;
			cr[4] = &cr4.raw;
			for(int i=0; i<5; i++)
				set_crn(i, 0);
		};

		uint32_t get_crn(int n) { return *cr[n]; };
		void set_crn(int n, uint32_t v) { *cr[n] = v; };
	protected:
		bool is_protected(void) { return cr0.PE; };
		bool is_ena_paging(void) { return cr0.PG; };
		uint32_t get_pdir_base(void) { return cr3.PageDirBase; };
};

#endif
