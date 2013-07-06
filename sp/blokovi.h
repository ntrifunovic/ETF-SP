// blokovi.h

#ifndef _BLOKOVI_H_
#define _BLOKOVI_H_

#include "common.h"
#include "prevedi.h"

// proverava da li je lokacija u bloku koji se trenutno izvrsava
// i brise blok iz kesa koji sadrzi datu lokaciju (ne mora biti blok koji se trenutno izvrsava)
bool self_modifying(uword location);

komanda getBlock(uword location);

#endif // _BLOKOVI_H_
