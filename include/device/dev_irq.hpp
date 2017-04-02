#ifndef _DEV_IRQ_H
#define _DEV_IRQ_H

#include "common.hpp"

class IRQ {
	protected:
		bool intr;
	public:
		IRQ() { intr = false; };
		virtual bool chk_intreq(void) { if(intr){ intr = false; return true; } return false; };
};

#endif
