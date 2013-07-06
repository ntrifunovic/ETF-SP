// jit.cpp

// Exit codes:
//	0	-	halt
//	1	-	losi atgumenti
//	2	-	ne postojeci fajl
//	3	-	ne postojeci opcode
//	4	-	destinacija nije validna
//  5 - TCC greska

#include "common.h"
#include "blokovi.h"
#include "exit.h"
#include "log.h"

//---------------
//--entry point--
//---------------
int main(int argc, char* argv[]) {	
	initLog();

	log("Interpreter pokrenut");

	if(argc != 2)
		WRONG_ARGUMENT_COUNT();
	
	if(!loadExecutableFile(argv[1]))
		FILE_CAN_NOT_BE_LOADED();

	log("Izvrsni fajl ucitan");

	for(;;) {
		log("IP: 0x%04x", IP);
		getBlock(IP)();
	}
}
