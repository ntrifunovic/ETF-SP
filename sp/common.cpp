// common.cpp

#include <stdio.h>

#include "common.h"
#include "log.h"

ubyte memory[MEMORY_SIZE];
CPU cpu;

bool loadExecutableFile(char* fileName) {
	FILE* f = fopen(fileName, "r");
	
	if(f == NULL)
		return false;
	
	// Ucitavamo izvrsni fajl u memoriju emulirane masine
	int size = fread(memory, 1, MEMORY_SIZE, f);
	
	log("Velicina ulaznog fajla %dB", size);

	fclose(f);
	
	return true;
}
