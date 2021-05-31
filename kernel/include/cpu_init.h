#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
struct gd {
  uint32_t base;
  uint32_t limit;
  uint8_t type;
};

struct id {
  uint32_t offset;
  uint16_t selector;
  uint8_t type_attr;
};

/*la vrai structure des id*/
struct ID_encoded {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
};

void PIC_remap(int offset1, int offset2);
uint8_t receivePs2Answer(void);
int initPs2(void);
void initPicMasks(void);
void initPIT(void);
