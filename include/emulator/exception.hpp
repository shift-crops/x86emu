#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#define EXP_DE 0
#define EXP_DB 1
#define EXP_BP 3
#define EXP_OF 4
#define EXP_BR 5
#define EXP_UD 6
#define EXP_NM 7
#define EXP_DF 8
#define EXP_TS 10
#define EXP_NP 11
#define EXP_SS 12
#define EXP_GP 13
#define EXP_PF 14
#define EXP_MF 16
#define EXP_AC 17
#define EXP_MC 18
#define EXP_XF 19
#define EXP_VE 20
#define EXP_SX 30

#define EXCEPTION(n, c)		if(c){ WARN("exception interrupt %d (%s)", n, #c); throw n; }
#define EXCEPTION_WITH(n, c, e)	if(c){ WARN("exception interrupt %d (%s)", n, #c); e; throw n; }

#endif