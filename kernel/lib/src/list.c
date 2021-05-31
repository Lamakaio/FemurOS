#include <stddef.h>
#include "kmalloc.h"
#include "list.h"
#include "terminal.h"
//those are python "lists", meaning chained arrays. They double capacity when full.
//Accessing an element i, appending or popping an element are all O(log(len(l))) (due to the malloc implementation, the search for a new block of size 2**n
//is always fast, as 2**n is an entrypoint to the free list for every n)

list empty_list(void) { //le premier tableau fait 7 de long, pour 4 éléments : il contient le pointeur suivant ainsi que la longueur de la liste.
  list ret = kmalloc(7*4);
  ret[5] = 0;
  ret[4] = (int) NULL;
  return ret;
}

int len(list l) {
  return l[5];
}

int get(list l, int i) {
  if (len(l) <= i) {
    return -1;
  }
  int log = 4;
  list parcours_l = l;
  while(i >= log) {
    i -= log;
    parcours_l = (list) parcours_l[log];
    log = 2*log;
  }
  return parcours_l[i];
}

void append(list l, int i) {
  int size = len(l);
  int log = 4;
  list parcours_l = l;
  while(size >= log) {
    size -= log;
    if (parcours_l[log] == 0) {
      parcours_l[log] = (int) kmalloc(4*(2*log + 1));
      ((int*) parcours_l[log])[2*log] = 0;
    }
    parcours_l = (list) parcours_l[log];
    log = 2*log;
  }
  parcours_l[size] = i;
  l[5]++;
}

int pop(list l) {
  int size = len(l)-1;
  int log = 4;
  list parcours_l = l;
  while(size >= log) {
    size -= log;
    parcours_l = (list) parcours_l[log];
    log = 2*log;
  }
  if (size == log - 1) {
    if (parcours_l[log]) {
      parcours_l[log] = 0;
    }
  }
  l[5]--;
  return parcours_l[size];
}

void delListRec(list l, int size, int log) {
  if (size < log) {
    kfree(l);
  }
  else {
    delListRec((int *) l[log], size - log, 2*log);
    kfree(l);
  }
}

void delList(list l) {
  delListRec(l, len(l), 4);
}
