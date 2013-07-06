// writeProgram.h

#include<stdio.h>
#include<string.h>

#include "log.h"
#include "common.h"
#include "exit.h"
#include "debug.h"

#define MAX_COMMENT_SIZE 100

void EXIT() {
  closeLog();
}

int main(int argc, char* argv[]) {
	initLog();

	if(argc != 3)
		WRONG_ARGUMENT_COUNT();

	FILE* f = fopen(argv[1], "r");
	
	if(f == NULL) 
		FILE_CAN_NOT_BE_LOADED();

	int pos = 0;

	char comments[MAX_COMMENT_SIZE];

	while(fscanf(f, "%x", memory + pos) != EOF) {
		pos++;

		if(fgetc(f) == ';') {
			fgets(comments, MAX_COMMENT_SIZE, f);
			log("comment: %s", comments);
		}
	}

	fclose(f);

	f = fopen(argv[2], "w");

	if(f == NULL) 
		FILE_CAN_NOT_BE_LOADED();

	fwrite(memory, 1, pos, f);
	
	fclose(f);

	#ifdef DEBUG
	f = fopen(argv[2], "r");

	int size = fread(memory, 1, MEMORY_SIZE, f);

	logMemory(0, size);
	#endif

	EXIT();

	return 0;
}
