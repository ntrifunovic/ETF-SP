// prevedi.h

#ifndef _PREVEDI_H_
#define _PREVEDI_H_

#include <string>
#include <sstream>

#include "common.h"
#include "komanda.h"
#include "log.h"

using namespace std;

void* prevedi(const char* code, const char* name);
void freeCodeMemory();

inline void reset(stringstream &ss) { 
	ss.str(string());

	ss	<< showbase << hex;

	ss << "#include \"common.h\"" << endl
		 << "#include \"exit.h\"" << endl
		 << "#include \"prevedi_blok.h\"" << endl
		 << endl
		 << "void f() {" << endl;
}

inline komanda prevedi(stringstream &ss) {
	ss << "}";

	string code = ss.str();

	log("GENERATED CODE:\n\n%s\n", code.c_str());
	
  return (komanda)prevedi(code.c_str(), "f");
}

inline ostream &operator << (ostream& out, ubyte u) {
	return out << (int)u;
}

#endif // _PREVEDI_H_
