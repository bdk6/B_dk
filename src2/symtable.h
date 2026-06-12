//#include <stdint.h>
#include "definitions.h"




word findSym(word* name, word isLocal);
word insertSym(word * sym, word isLocal);
word symsetadr(word entry, word addr);
word symgetadr(word entry);
word symsetval(word entry, word val);
word symgetval(word entry);
word symsetclass(word entry, word cls);
word symgetclass(word entry);
word symsetdefined(word entry, word def);
word symgetdefined(word entry);
word removeLocals(void);
word symgetlocal(word entry);


