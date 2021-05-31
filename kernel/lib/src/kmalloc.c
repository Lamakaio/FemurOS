#include <stdint.h>
#include "kmalloc.h"
#include "printf.h"
//voir le rapport pour la description de l'implémentation de malloc

void* freeListQueue = NULL;
void* entryPoints[32] = {NULL};
void* lastAddr = NULL;

void* clear(void *addr) {
  return (void*) ((int) addr & ~3);
}
void* intPlusPointer(int i, void* addr) {
  return (void*) (((int) addr) + i);
}
int log2(int n) { //logarithme en base 2
  uint8_t log = 0;
  if (n <= 0) return 0;
  while (n-1) {
    n = n >> 1;
    log++;
  }
  return log;
}

void initKmalloc(void* bottom) {
  heapBottom = bottom;
  heapTop = bottom;
}

void* sbrk(int increment) {
  void *ret = heapTop;
  heapTop = intPlusPointer((increment + 3) & ~3, heapTop); //arrondi au supérieur (pour un multiple de 4)
  return ret;
}

int brk(void* address) { //l'adresse doit être un multplie de 4. Sinon, la fonction l'arrondi brutalement.
  heapTop = clear(address);
  return 0;
}

int newBlock(int taille) {
  int addr = (int) sbrk(taille);
  if (lastAddr && ((* (int *) lastAddr) & 1)) {
      (* (void **) addr) = (void*) (addr + taille + 2);
  }
  else {
    (* (void **) addr) = (void*) (addr + taille);
  }
  lastAddr = (void*) addr;
  return addr + 4;
}

void insertBlock(void* addr, int taille) {
  int logTaille = log2(taille);
  void *succ = entryPoints[logTaille]; //insertion dans la free list
  void *pred;
  if (succ) pred = clear(* (void **) (intPlusPointer(8, succ))); //si le succ est nul le pred est le dernier élément de la liste chainée.
  else {
    pred = freeListQueue;
    freeListQueue = addr;
  }
  if (pred) * (void **) intPlusPointer(4, pred) = addr; //successeur du prédecesseur, si le pred n'est pas NULL
  if (succ) * (void **) intPlusPointer(8, succ) = addr; //prédecesseur du successeur, si le succ n'est pas NULL
  * (void **) intPlusPointer(4, addr) = succ; //successeur
  * (void **) intPlusPointer(8, addr) = pred; //prédecesseur
  while (logTaille >= 0 && (entryPoints[logTaille] == succ || entryPoints[logTaille] == NULL)) {
    entryPoints[logTaille] = addr;
    logTaille --;
  }
}

void delFreeBlock(void* addr) { //suppression d'un bloc libre
  if (addr == freeListQueue) {
    freeListQueue = clear(* (void **) addr);
  }
  int i = 32;
  while (--i >= 0) { //suppression des entrypoints
    if (entryPoints[i] == addr) {
        entryPoints[i] = * (void **) intPlusPointer(4, addr);
    }
  }
  void *nextAddr = * (void **) intPlusPointer(4, addr); //mise à jour de la free list
  void *prevAddr = * (void **) intPlusPointer(8, addr);
  if (prevAddr) * (void **) intPlusPointer(4, prevAddr) = nextAddr; //entete du successeur du prédecesseur
  if (nextAddr) * (void **) intPlusPointer(8, nextAddr) = prevAddr; //prédecesseur du successeur


}

void* kmalloc(uint32_t taille) {
  if (taille == 0) {
    return NULL;
  }
  if (taille << 30) { //si la taille n'est pas un multiple de 4, on arrondi au supérieur.
    taille = (taille + 3) & ~3;
  }
  if (taille < 16) { //la taille minimale est 12 octets
    taille = 16;
  }
  taille += 4; //place pour l'entete
  int logTaille = log2(taille);
  #pragma GCC diagnostic ignored "-Warray-bounds"
  //comme taille >= 12, logTaille >= 1 et donc il n'y a pas de problème d'indice.
  if (entryPoints[logTaille - 1] == NULL) { //si il n'y a pas de blocs libres de la bonne taille
    return (void*) newBlock(taille);
  }
  else {
    void *addr = entryPoints[logTaille - 1];
    while((uint32_t) clear(* (void **) addr) - (uint32_t) addr < taille) { //si la taille du bloc est suffisante (&~3 = arrondi à 4)
      addr = * (void **) intPlusPointer(4, addr); //on parcours la liste
      if (addr == NULL) {
        return (void*) newBlock(taille);
      }
    }
    int tailleBloc = (uint32_t) clear(* (void **) addr) - (uint32_t) addr;
    int entete;
    if (tailleBloc - taille >= 12) {
      * (void **) intPlusPointer(taille, addr) = clear(* (void **) addr); //entête de début
      * (void **) intPlusPointer(tailleBloc - 4, addr) = * (void **) intPlusPointer(taille, addr); //entête de fin
      insertBlock(intPlusPointer(taille, addr), tailleBloc - taille);
      entete = taille;
    }
    else {
      entete = tailleBloc;
      if (* (void **) intPlusPointer(4, addr)) {
        int * prevBlockStart = clear(* (void **) intPlusPointer(4, addr));
        *prevBlockStart = (*prevBlockStart) & ~2; //le bloc précédent n'est plus libre dans ce cas
      }
    }
    delFreeBlock(addr);
    * (void **) addr = intPlusPointer(entete, addr); //entête de début du bloc alloué
    return intPlusPointer(4, addr);
  }
}

void kfree(void* addr) {
  if (addr == NULL) return;
  addr = intPlusPointer(-4, addr); //correction pour inclure l'entête
  * (int *) (addr) = * (int *) (addr) | 1;
  int * nextBlockStart = clear(* (void **) (addr));
  if (lastAddr != addr) { //si il y a un bloc suivant
    * nextBlockStart = * nextBlockStart | 2; //mise à jour de l'entête du bloc suivant
    if (*nextBlockStart & 1) { //si le bloc suivant est un bloc libre
      nextBlockStart = clear((void*) *nextBlockStart);
      delFreeBlock(clear(* (void**) addr));
      * (int *) (addr) = (int) intPlusPointer(1, nextBlockStart); //entête de début comprenant le bloc suivant
    }
  }
  if ((* (int *) addr) & 2) { //si le bloc précédent est libre
    addr = * (void **) intPlusPointer(-4, addr);
    delFreeBlock(addr);
    * (int *) (addr) = (int) intPlusPointer(1, nextBlockStart); //entete de début contenant le bloc de base et le précédent
  }
  * (void **) intPlusPointer(-4, nextBlockStart) = clear(addr); //entete de fin
  int taille = clear(* (void **) (addr)) - addr; //taille après les fusions
  insertBlock(addr, taille);
}

void* krealloc(void* addr, uint32_t taille) { //ne marche pas bien
  void *new_addr = kmalloc(taille);
  int oldTaille = clear( *((void **) intPlusPointer(-4, addr))) - addr;
  for (int i = 0; i < oldTaille; i+=4) {
    *((void **) intPlusPointer(i, new_addr)) = *((void **) intPlusPointer(i, addr));
  }
  kfree(addr);
  return new_addr;
}

void* kcalloc(uint32_t taille) {
  void* addr = kmalloc(taille);
  for (uint32_t i = 0; i < taille; i+=4) *((void**) intPlusPointer(i, addr)) = 0;
  return addr;
}

void* kmalloc_align(uint32_t taille, uint32_t align) {
  sbrk(align - ((((int) heapTop) + 4)%align));
  return (void*) newBlock(taille + 4);
}
