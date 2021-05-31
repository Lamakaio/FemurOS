#include "kmalloc.h"
#include "bitset.h"

//implémentation de bitset (tableau de booléen efficace en espace en gros)

struct Frame *initFrame(uint32_t size) {
  bitset b = kcalloc(size/32);
  struct Frame *ret = kmalloc(sizeof(struct Frame));
  ret->set = b;
  ret->lastFound = 0;
  ret->size=size;
  return ret;
}

void setBit(struct Frame *f, uint32_t loc) {
  f->set[loc/32] |= (1<<(loc%32));
}

void clearBit(struct Frame *f, uint32_t loc) {
  f->set[loc/32] &= ~(1<<(loc%32));
}

int testBit(struct Frame *f, uint32_t loc) {
  return f->set[loc/32] & (1<<(loc%32));
}

//la recherche reprend la ou elle s'était arrété, ce qui est bien plus efficace
//dans le cas d'utilisation actuel, ou quasiment tout est utilsé jusqu'à un certain point
uint32_t findFirstClear(struct Frame *f) {
  while (f->lastFound <= f->size && f->set[f->lastFound/32] == 0xFFFFFFFF) {
    if (f->lastFound == f->size) return f->lastFound = 0;
    f->lastFound = (f->lastFound + 32) & (~31);
  }
  while (testBit(f, f->lastFound)) f->lastFound++;
  setBit(f, f->lastFound);
  return f->lastFound;
}
