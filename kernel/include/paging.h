#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct PageTable* PageDirectory;
struct Page {
  uint32_t present : 1;
  uint32_t writeEnabled : 1;
  uint32_t userAuth : 1;
  uint32_t writeThrough : 1;
  uint32_t cacheDisable : 1;
  uint32_t accessed : 1;
  uint32_t dirty : 1;
  uint32_t mustBeZero : 1;
  uint32_t global : 1;
  uint32_t freeSpace : 3;
  uint32_t pageAdress : 20;
};

struct PageTable {
  uint32_t present : 1;
  uint32_t writeEnabled : 1;
  uint32_t userAuth : 1;
  uint32_t writeThrough : 1;
  uint32_t cacheDisable : 1;
  uint32_t accessed : 1;
  uint32_t mustBeZero : 1;
  uint32_t pageSize : 1; //0 for 4KiB
  uint32_t ignored : 1;
  uint32_t freeSpace : 3;
  uint32_t pageAdress : 20;
};

void initPaging(uint32_t maxMem);
int getCurrentDir();
PageDirectory newPageDirectory(int userAuth);
void alloc_page(uint32_t virtualAddr, PageDirectory pd);
void freePageDirectory(PageDirectory pd);
