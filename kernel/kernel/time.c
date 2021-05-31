#include <stdint.h>
#include "time.h"
uint32_t timer = 0;

int getTime(void) {return timer;}
void incTime(void) {timer++;}
