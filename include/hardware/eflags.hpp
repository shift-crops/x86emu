#ifndef _EFLAGS_H
#define _EFLAGS_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "common.hpp"
#include "processor.hpp"

#define is_x(XF) 	processor && processor->eflags.XF
#define set_x(XF, f) 	do{ assert(processor); processor->eflags.XF = f; }while(0)

class Eflags {
	private:
		Processor *processor;

	public:
		Eflags() { processor = NULL; };
		Eflags(Processor *p);

		bool is_carry(void){ return is_x(CF); };
		bool is_parity(void){ return is_x(PF); };
		bool is_zero(void){ return is_x(ZF); };
		bool is_sign(void){ return is_x(SF); };
		bool is_overflow(void){ return is_x(OF); };
		void set_carry(bool carry){ set_x(CF, carry); };
		void set_parity(bool parity){ set_x(PF, parity); };
		void set_zero(bool zero){ set_x(ZF, zero); };
		void set_sign(bool sign){ set_x(SF, sign); };
		void set_overflow(bool over){ set_x(OF, over); };

		template <class T> uint32_t update_add(T v1, uint32_t v2);
		template <class T> uint32_t update_or(T v1, uint32_t v2);
		template <class T> uint32_t update_and(T v1, uint32_t v2);
		template <class T> uint32_t update_sub(T v1, uint32_t v2);
	private:
		bool chk_parity(uint8_t v);
};

#endif
