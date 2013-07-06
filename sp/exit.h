// exit.h

// Exit codes:
//	0	-	halt
//	1	-	losi atgumenti
//	2	-	ne postojeci fajl
//	3	-	ne postojeci opcode
//	4	-	destinacija nije validna
//  5 - TCC greska

#ifndef _EXIT_H_
#define _EXIT_H_

#include <stdlib.h>

#include "log.h"

#ifndef NO_EXIT
// ovu metodu treba implementirati ako treba osloboditi resurse
// pre zavrsetka programa
void EXIT();
#endif

inline void HALT() {
	log("HALT");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(0);
}
inline void WRONG_ARGUMENT_COUNT() {
	log("WRONG_ARGUMENT_COUNT");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(1);
}
inline void FILE_CAN_NOT_BE_LOADED() {
	log("FILE_CAN_NOT_BE_LOADED");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(2);
}
inline void WRONG_OP_CODE() {
	log("WRONG_OP_CODE");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(3);
}
inline void ADRESSING_ERROR() {
	log("ADRESSING ERROR");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(4);
}
inline void TCC_ERROR() {
	log("TCC ERROR");
	#ifndef NO_EXIT
	EXIT();
  #endif
	exit(5);
}

#endif // _EXIT_H_
