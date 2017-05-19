#ifndef _EXCEPTION_H
#define _EXCEPTION_H

enum exception_t {
	EXP_DE = 0,
	EXP_DB = 1,
	EXP_BP = 3,
	EXP_OF = 4,
	EXP_BR = 5,
	EXP_UD = 6,
	EXP_NM = 7,
	EXP_DF = 8,
	EXP_TS = 10,
	EXP_NP = 11,
	EXP_SS = 12,
	EXP_GP = 13,
	EXP_PF = 14,
	EXP_MF = 16,
	EXP_AC = 17,
	EXP_MC = 18,
	EXP_XF = 19,
	EXP_VE = 20,
	EXP_SX = 30
};

#define EXCEPTION(n, c)		if(c){ WARN("exception interrupt %d (%s)", n, #c); throw n; }
#define EXCEPTION_WITH(n, c, e)	if(c){ WARN("exception interrupt %d (%s)", n, #c); e; throw n; }

#endif
