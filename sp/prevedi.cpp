// prevedi.cpp

#include <list>

#include "libtcc.h"

#include "prevedi.h"
#include "prevedi_blok.h"
#include "blokovi.h"
#include "exit.h"
#include "log.h"
#include "debug.h"

using namespace std;

list<void*> codeMemory;

void freeCodeMemory() {
	for(list<void*>::iterator i = codeMemory.begin(); i != codeMemory.end(); i++)
		free(*i);
}

void* prevedi(const char* code, const char* name) {
	TCCState *s = tcc_new();

	if (!s) {
		log("Could not create tcc state");
		TCC_ERROR();
	}
	
	// if tcclib.h and libtcc1.a are not installed, where can we find them
	//if (argc == 2 && !memcmp(argv[1], "lib_path=",9))
	//	tcc_set_lib_path(s, argv[1]+9);
	
	// MUST BE CALLED before any compilation
	tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

  tcc_add_include_path(s, "sp/"); // TBD
	//tcc_define_symbol(s, "LOG", "");

	if (tcc_compile_string(s, code) == -1)
		TCC_ERROR();

  tcc_add_symbol(s, "cpu", &cpu);
  tcc_add_symbol(s, "memory", &memory);
  #ifdef LOG
  tcc_add_symbol(s, "vlog", (void*)vlog);
  #endif
	#ifdef DEBUG
	tcc_add_symbol(s, "logMemory", (void*)logMemory);
	tcc_add_symbol(s, "logCPUInfo", (void*)logCPUInfo);
	tcc_add_symbol(s, "logStackTrace", (void*)logStackTrace);
	#endif
	#ifdef SELF_MODIFYING

	komanda  (*_prevedi)(const uword) = prevedi;
	tcc_add_symbol(s, "prevedi", (void*)_prevedi);
	tcc_add_symbol(s, "self_modifying", (void*)self_modifying);
	#endif
	tcc_add_symbol(s, "EXIT", (void*)EXIT);

	// get needed size of the code 
	int size = tcc_relocate(s, NULL);
	if (size == -1)
		TCC_ERROR();
	
	// allocate memory and copy the code into it
	void* mem = malloc(size);

  codeMemory.push_back(mem);

	tcc_relocate(s, mem);

  void* ret = tcc_get_symbol(s, name);
	// delete the state
	tcc_delete(s);

  return ret;
}
