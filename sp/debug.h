// debug.h

// Ako zelimo da koristimo debug-mod moramo iskompajlirati sa -D DEBUG
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "common.h"

void logMemory(uword start, uword size);
void logCPUInfo();
void logStackTrace();

#endif // _DEBUG_H_
