// blokovi.cpp

#include <map>

#include "blokovi.h"
#include "common.h"
#include "log.h"
#include "prevedi_blok.h"

using namespace std;

map<uword, komanda> codeLocation; // masinski kod

#ifdef SELF_MODIFYING
map<uword, uword> startEnd; // kraj mapira u pocetak bloka

bool self_modifying(uword location) {
	log3("self_modifying| location:%04hx", location);

	map<uword, uword>::iterator it = startEnd.upper_bound(location);

	if(it == startEnd.end() || it->first <= location )
		return false;

	codeLocation.erase(it->first);
	
	log2("self_modifying| deleting block (start: %04hx, end: %04hx)", it->second, it->first);

	return it->second == IP;
}
#endif

komanda getBlock(uword location) {
  if(codeLocation.find(location) == codeLocation.end()) {
		codeLocation[location] = prevedi(location);

		log2("block translated| start=%04hx end=%04hx", location, IP);

    #ifdef SELF_MODIFYING
		startEnd[IP] = location;
		// za self_modifying metodu je neophodno da IP pokazuje na pocetak bloka
		IP = location;
		#endif
	}
		
  return codeLocation[location];
}
