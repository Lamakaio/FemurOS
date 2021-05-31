#include <stdint.h>
typedef uint32_t* bitset;
struct Frame {
  bitset set;
  uint32_t lastFound;
  uint32_t size;
};
struct Frame *initFrame(uint32_t size);
void setBit(struct Frame *b, uint32_t loc);
void clearBit(struct Frame *b, uint32_t loc);
int testBit(struct Frame *b, uint32_t loc);
uint32_t findFirstClear(struct Frame *b);
