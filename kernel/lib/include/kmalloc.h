#include <stddef.h>
#include <stdint.h>
void* heapBottom;
void* heapTop;

void initKmalloc(void* bottom);
void* kmalloc(uint32_t taille);
void* krealloc(void* addr, uint32_t taille);
void kfree(void* addr);
void* kcalloc(uint32_t taille);
void* kmalloc_align(uint32_t taille, uint32_t align);
