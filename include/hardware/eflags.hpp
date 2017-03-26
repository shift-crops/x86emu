#ifndef _EFLAGS_H
#define _EFLAGS_H

#include <stdint.h>
#include "common.hpp"

class Eflags {
	private:
		union {
			uint32_t reg32;
			uint16_t reg16;

			struct {
				uint32_t CF : 1;
				uint32_t : 1;           // 1
				uint32_t PF : 1;
				uint32_t : 1;           // 0
				uint32_t AF : 1;
				uint32_t : 1;           // 0
				uint32_t ZF : 1;
				uint32_t SF : 1;
				uint32_t TF : 1;
				uint32_t IF : 1;
				uint32_t DF : 1;
				uint32_t OF : 1;
				uint32_t IOPL : 2;
				uint32_t NT : 1;
				uint32_t : 1;           // 0
				uint32_t RF : 1;
				uint32_t VM : 1;
				uint32_t AC : 1;
				uint32_t VIF : 1;
				uint32_t VIP : 1;
				uint32_t ID : 1;
			};
		} eflags;

	public:
		Eflags();

		uint32_t get_eflags(void){ return eflags.reg32; };
		void set_eflags(uint32_t v){ eflags.reg32 = v; };
		uint16_t get_flags(void){ return eflags.reg16; };
		void set_flags(uint16_t v){ eflags.reg16 = v; };

		bool is_carry(void){ return eflags.CF; };
		bool is_parity(void){ return eflags.PF; };
		bool is_zero(void){ return eflags.ZF; };
		bool is_sign(void){ return eflags.SF; };
		bool is_overflow(void){ return eflags.OF; };
		void set_carry(bool carry){ eflags.CF = carry; };
		void set_parity(bool parity){ eflags.PF = parity; };
		void set_zero(bool zero){ eflags.ZF = zero; };
		void set_sign(bool sign){ eflags.SF = sign; };
		void set_overflow(bool over){ eflags.OF = over; };

		template <class T> uint32_t update_eflags_add(T v1, uint32_t v2);
		template <class T> uint32_t update_eflags_or(T v1, uint32_t v2);
		template <class T> uint32_t update_eflags_and(T v1, uint32_t v2);
		template <class T> uint32_t update_eflags_sub(T v1, uint32_t v2);
	private:
		bool chk_parity(uint8_t v);
};

#endif
