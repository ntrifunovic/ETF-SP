// debug.cpp

#include "debug.h"
#include "log.h"
#include "common.h"

void logMemory(uword start, uword size) {
  #if DEBUG
  log("-------------MEMORY-----------------");
	const int showOnLine = 10;
	char line[5*showOnLine+1];

	for(int i = 0; i < size; i++) {
		if(i%showOnLine == 0) {
			if(i > 0)
				log("%s", line);
			log("memory[0x%04hx] ...", start+i);
		}

	 	sprintf(line + (i%showOnLine)*5, "\t0x%02hhx\0", read_ubyte(start+i));
	}
	
	log("%s", line);
  log("-------------------------------------");
	#endif
}

void logCPUInfo() {
  #if DEBUG
  log("--------------CPU INFO--------------");

  log("|IP:0x%04hx|\tSP:0x%04hx\tPSW:0x%04hx\n", IP, SP, PSW);
  log("\tZ:%d\tC:%d\tN:%d\tO:%d", TEST_FLAG_Z(), TEST_FLAG_C(), TEST_FLAG_N(), TEST_FLAG_O());
  log("----------------REGS----------------");


	const int showOnLine = 5;
	char regsInfo[7*showOnLine+1];
	char header[6*showOnLine+1];

	for(int i = 0; i < REGS_COUNT; i++) {
    if(i%showOnLine == 0 && i > 0) {
      log("%s", header);
      log("%s", regsInfo);
     }
		sprintf(regsInfo + (i%showOnLine)*7, "0x%04hx\t\0", REGS[i]);
		sprintf(header + (i%showOnLine)*6, "r[%2d]\t\0", i);
	}

  log("%s", header);
  log("%s", regsInfo);
  log("------------------------------------");
	#endif
}

void logStackTrace() {
  #if DEBUG
  log("----------------STACK---------------");
  logMemory(SP, MEMORY_SIZE - (udword)SP);
	#endif
}
