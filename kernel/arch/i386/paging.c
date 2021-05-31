#include "bitset.h"
#include "paging.h"
#include "kmalloc.h"
#include "list.h"
#include "printf.h"
#include "keyboard.h"


struct Frame *framesTable; //bitset des pages physiques (libres ou occupées)
struct Page __attribute__ ((aligned (4096))) kernelPage1[1024]; //tables de pages du kernel.
struct Page __attribute__ ((aligned (4096))) kernelPage2[1024];
PageDirectory currentDir;
list freePageDirectories; //plus utilisé

int getCurrentDir() {return (int) currentDir;}

struct Page makePageEntry(uint32_t addr, int writeEnabled, int userAuth) {
  struct Page ret = {1, writeEnabled, userAuth, 1, 0, 0, 0, 0, 0, 0, addr >> 12};
  return ret;
}

void initKernelPages() {
  for (uint32_t i = 0; i < 1024; i += 1) {
    setBit(framesTable, i);
    kernelPage1[i] = makePageEntry(4096*i, 1, 0);
  }
  for (uint32_t i = 0; i < 1024; i += 1) {
    setBit(framesTable, 1024+i);
    kernelPage2[i] = makePageEntry(4096*(1024+i), 1, 0);
  }
}

struct PageTable makePageTable(uint32_t addr, int writeEnabled, int userAuth) {
  struct PageTable ret = {1, writeEnabled, userAuth, 1, 0, 0, 0, 0, 0, 0, addr >> 12};
  return ret;
}

void alloc_page(uint32_t virtualAddr, PageDirectory dir) {
  if (! (dir[virtualAddr >> 22].present)) {
    //printf("plop");
    struct Page *newPageTable = kmalloc_align(1024 * sizeof(struct Page), 4096);
    dir[virtualAddr >> 22] = makePageTable((uint32_t) newPageTable, 1, 0);
    //printf("~~%b %h~~", currentDir[virtualAddr >> 22].present, virtualAddr >> 22);
  }
  struct Page* pageAddr = (struct Page*)(dir[virtualAddr >> 22].pageAdress << 12);
  if (! (pageAddr->present & 1)) {
    int physAddr = findFirstClear(framesTable)*4096;
    pageAddr[virtualAddr >> 12 & 0x03FF] = makePageEntry(physAddr, 1, 0);
  }
}

void freePageDirectory(PageDirectory pd) {
  for (int i = 2; i < 1023; i++) {
    if (pd[i].present) {
      for (int j = 0; j < 1024; j++) {
        clearBit(framesTable, (pd[i].pageAdress >> 10) + j);
      }
      kfree((void*) (pd[i].pageAdress << 12));
    }
  }
  kfree(pd);
}

PageDirectory newPageDirectory(int userAuth) {
  PageDirectory pd = kmalloc_align(1024 * sizeof(struct PageTable), 4096);
  pd[0] = makePageTable((uint32_t) kernelPage1, 1, userAuth);
  pd[1] = makePageTable((uint32_t) kernelPage2, 1, userAuth);
  struct PageTable null = {0};
  for (int i = 2; i < 1023; i++) {
    pd[i] = null;
  }
  pd[1023] = makePageTable((uint32_t) pd, 1, 1);
  alloc_page(0xFF000000, pd);
  alloc_page(0xFEFFF000, pd);
  return pd;
}

void initPaging(uint32_t maxMem) {
  framesTable = initFrame(maxMem/4096);
  initKernelPages();
  PageDirectory pd = newPageDirectory(0);
  freePageDirectories = empty_list();
  currentDir = pd;
  printf("enabling paging... ");
  asm volatile("mov %0, %%cr3":: "r"(pd));
  uint32_t cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000001; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
  printf("done.\n");
}
