#include "shell.h"
#include "terminal.h"
#include "time.h"
#include "keyboard.h"
#include <stddef.h>
#include <stdbool.h>
#include "kmalloc.h"
#include "printf.h"
#include "list.h"
#include "kernel.h"
#define NBCOMMANDES 8
//paramètres obligatoire pour une fonction shell donc inutilisé si elle ne prend pas d'arguments
#pragma GCC diagnostic ignored "-Wunused-parameter"

void shellCommandProcess() {
  struct strWithLen (*function)(struct Args args) = (void*) receive(1); //recoit la donction à executer et sont argument
  struct Args *entree = (void*) receive(1);
  int id = receive(1);
  terminal_newline();
  struct strWithLen retour = function(*entree);
  send(10+id, 0); //signale que la fonction a fini de s'executer
  terminal_delline();
  terminal_write(retour.str, retour.len);
  terminal_newline();
  send(10+id, 0xDEAD); //signale que l'impression est terminée, il n'y a plus rien à faire.
  //le free fait parfoit crash l'os donc je l'ai commenté...
  /*for(int i = 0; i <= entree->numArgs; i++) {
    //printf(" %h ", entree->args[i].str);
    kfree(entree->args[i].str);
    //printf("$ %h $", entree->args);
  }
  //printf("plop");
  //printf("$ %h $", entree->args);
  kfree(entree->args);
  kfree(entree);*/
  exit();
}


//fonctions correspondant au différentes commandes.
struct strWithLen game_func(struct Args entree) {
  switch (entree.args[0].str[0]) {
    case 's':
    send(42, 0);
    receive(42);
    break;
  }
  struct strWithLen ret = {"", 0};
  return ret;
}
struct strWithLen exit_func(struct Args entree) {
  exit();
  struct strWithLen ret = {"done", 4};
  return ret;
}
struct strWithLen echo_func(struct Args entree) {
  return entree.args[0];
}
struct strWithLen pageFault_func(struct Args entree) {
  *((int*) 0x1fffffff) = 3;
  struct strWithLen ret = {"done", 4};
  return ret;
}
int parseInt(struct strWithLen str) {
  int x = 1;
  int ret = 0;
  if (str.str[0] == '-') {
    x = -1;
    str.str[0] = '0';
  }
  for (int i = str.len - 1; i >= 0; i--) {
    ret += x * (str.str[i] - 48);
    x = x * 10;
  }
  return ret;
}
struct strWithLen calc_func(struct Args entree) {
  int dep = parseInt(entree.args[0]);
  for (int i = 0; i < entree.args[1].len; i++) {
    char c = entree.args[1].str[i];
    if (c == 'a') {
      dep += parseInt(entree.args[2+i]);
    }
    if (c == 'm') {
      dep = dep*parseInt(entree.args[2+i]);
    }
    if (c == 'd') {
      dep = dep/parseInt(entree.args[2+i]);
    }
    if (c == 'M') {
      dep = dep%parseInt(entree.args[2+i]);
    }
  }
  struct strWithLen ret = sprintf("resultat : %d", dep);
  return ret;
}
struct strWithLen sleep_func(struct Args entree) {
  int t = getTime();
  int wait = parseInt(entree.args[0]);
  while (getTime() <= t + wait) switchToNextTask();
  struct strWithLen ret = {"done", 4};
  return ret;
}
bool isPrime(int p) {
  if (p%3 == 0) return false;
  for(int i = 6; i < p/12; i += 6) {
    if (p%(i+1) == 0 || p%(i-1) == 0) {
      return false;
    }
  }
  return true;
}
struct strWithLen prime_func(struct Args entree) {
  int n = parseInt(entree.args[0]);
  n = n - 1;
  uint32_t p = 1;
  while (--n >= 0) {
    p += 2;
    while(!isPrime(p)) p+=2;
  }
  return sprintf("%d", p);
}
struct Command echo = {};
struct Command game = {};
struct Command prime = {};
struct Command calc = {};
struct Command pageFault = {};
struct Command _exit = {};
struct Command sleep = {};
struct PrefixTree tree = {0};
struct Command commandes[NBCOMMANDES];


void initCommands(void){ //inittialisation de la table des commandes
  echo.text = "echo";
  echo.function = echo_func;
  echo.mandatoryArgs = 1;
  echo.opts = "";
  commandes[ECHO] = echo;
  calc.text = "calc";
  calc.function = calc_func;
  calc.mandatoryArgs = 1;
  calc.opts = "amdM";
  commandes[CALC] = calc;
  pageFault.text = "pagefault";
  pageFault.function = pageFault_func;
  pageFault.mandatoryArgs = 0;
  pageFault.opts = "";
  commandes[PAGEFAULT] = pageFault;
  _exit.text = "exit";
  _exit.function = exit_func;
  _exit.mandatoryArgs = 0;
  _exit.opts = "";
  commandes[EXIT] = _exit;
  sleep.text = "sleep";
  sleep.function = sleep_func;
  sleep.mandatoryArgs = 1;
  sleep.opts = "";
  commandes[SLEEP] = sleep;
  prime.text = "prime";
  prime.function = prime_func;
  prime.mandatoryArgs = 1;
  prime.opts = "";
  commandes[PRIME] = prime;
  game.text = "game";
  game.function = game_func;
  game.mandatoryArgs = 1;
  game.opts = "";
  commandes[GAME] = game;

}

struct PrefixTree* getChild(struct PrefixTree tree, char c) {
  for (int i = 0; i < tree.numChildren; i++) {
    if (tree.children[i]->letter == c) {
      return tree.children[i];
    }
  }
  return NULL;
}

void freeDFS(struct PrefixTree *currentTree) {
  if (currentTree->children == NULL) {
    return;
  }
  struct PrefixTree **newChildrenArray = kmalloc(4 * currentTree->numChildren);
  for (int i = 0; i < 34 && currentTree->children[i] != NULL; i++) {
    newChildrenArray[i] = currentTree->children[i];
    freeDFS(currentTree->children[i]);
  }
  kfree(currentTree->children);
  currentTree->children = newChildrenArray;
}

void makeTree(void) { //arbre des fonctions
  //première passe : on alloue sans optimiser pour la construction
  struct PrefixTree *currentTree;
  for (int i_com = 1; i_com < NBCOMMANDES; i_com++) {
    int len = strlen(commandes[i_com].text);
    currentTree = &tree;
    for (int i_str = 0; i_str < len; i_str++) {
      if (currentTree->children == NULL) {
        currentTree->children = kcalloc(4*34);
      }
      struct PrefixTree* child = getChild(*currentTree, commandes[i_com].text[i_str]);
      if (child == NULL) {
        child = kcalloc(16);
        child->id = NONE;
        child->letter = commandes[i_com].text[i_str];
        child->children = NULL;
        child->numChildren = 0;
        currentTree->children[currentTree->numChildren] = child;
        currentTree->numChildren++;
      }
      currentTree = child;
    }
    currentTree->id = i_com;
  }
  freeDFS(&tree);
}

struct strWithLen parseWord(list line, int indice) {
  int n = 0;
  int temp = indice - 1;
  while (++temp < len(line) && get(line, temp) != ' ') {
    n++;
  }
  char* str = kmalloc(n);
  int longueur = n;
  while (--n >= 0) {
    str[n] = get(line, indice + n);
  }
  //printf("%h", str);
  struct strWithLen ret = {str, longueur};
  return ret;
}

struct Args *parseArgs(list line, int indice, enum commandes id) {
  struct Command currentCommand = commandes[id];
  int nbArgs = currentCommand.mandatoryArgs;
  int nbOpts = 0;
  int maxOpts = strlen(currentCommand.opts);
  int iMandatoryArgs = 0;
  int bg = 0;
  struct strWithLen *mandatoryArgs = kmalloc(8*nbArgs);
  struct strWithLen *opts = kcalloc(8*strlen(currentCommand.opts));
  while(indice < len(line)) {
    char c = get(line, indice);
    if (c == '-') { //cas d'une option
      if (++indice >= len(line)) {
        printf("Arguments invalides : une commande ne peut finir par -\n");
      }
      else {
        char opt = get(line, indice);
        indice++;
        int k;
        for(k = 0; k < maxOpts && currentCommand.opts[k] != opt; k++);
        if (k == maxOpts) printf("Argument inconnu : %c\n", opt);
        else {
          opts[k] = parseWord(line, indice);
          indice += opts[k].len;
          nbOpts++;
        }
      }
    }
    else if (c == '&') { //cas du multitasking
      bg = true;
      break;
    }
    else if (c != ' ') { //cas d'un argument obligatoire
      if (iMandatoryArgs >= nbArgs) {
        printf("Erreur d'argument : mauvaise arite\n");
      }
      else {
        mandatoryArgs[iMandatoryArgs] = parseWord(line, indice);
        indice += mandatoryArgs[iMandatoryArgs].len;
        iMandatoryArgs++;
      }
    }
    indice++;
  }
  struct strWithLen *allArgs = kmalloc(sizeof(struct strWithLen)*(nbArgs + nbOpts + 1));
  for (int i = 0; i < nbArgs; i++) allArgs[i] = mandatoryArgs[i];
  kfree(mandatoryArgs);
  int iOpts = 0;
  int iArgs = 0;
  struct strWithLen optsIndicator;
  optsIndicator.str = kmalloc(nbOpts);
  optsIndicator.len = nbOpts;
  while(iArgs < nbOpts) {
    while(opts[iOpts].str == 0) iOpts++;
    allArgs[nbArgs + iArgs + 1] = opts[iOpts];
    optsIndicator.str[iArgs] = currentCommand.opts[iOpts];
    iOpts++;
    iArgs++;
  }
  allArgs[nbArgs] = optsIndicator;
  kfree(opts);
  struct Args *ret = kmalloc(2*sizeof(struct Args));
  ret->args = allArgs;
  ret->numArgs = nbArgs + nbOpts;
  ret->background = bg;
  return ret;
}

void parseExecCommand(list command, int id) {
  char c = get(command, 0);
  int i = 0;
  struct PrefixTree *currentTree = &tree;
  do {
    currentTree = getChild(*currentTree, c);
    c = get(command, ++i);
  } while(i < len(command) && c != ' ');
  if (currentTree == NULL || currentTree->id == NONE) {
    printf("\nCommande inconnue\n");
    return;
  }
  else {
    struct Args *args = parseArgs(command, i, currentTree->id);
    int bg = args->background;
    newProcess(shellCommandProcess, getCurrentProcess()->id, 1, 1, 0);
    send(1, (int) commandes[currentTree->id].function);
    send(1, (int) args);
    send(1, id);
    if (!bg) {
      while (probe(10+id) != Sender || receive(10+id) != 0xDEAD) {
        if (getKey(LCTRL) && getKey(C)) {
          printf("ctrlC");
          return;
        }
        if (probe(0) == Sender) {
          return;
        }
        switchToNextTask();
      }
    }
  }
}

void exec_shell(int id, list command) {
  int blink = 0;
  while(1) {
    if (getTime()%100 < 50 && blink == 0) {
      blink = 1;
      terminal_delchar();
      terminal_putchar(219);
    }
    if (getTime()%100 >= 50 && blink == 1) {
      blink = 0;
      terminal_delchar();
      terminal_putchar(' ');
    }
    char c = getChar();
    if (c == '\n') {
      terminal_delchar();
      parseExecCommand(command, id);
      flushBuf();
      delList(command);
      command = empty_list();
      printf("plush %d$ ", id-1);
    }
    else if (c == 8 && terminal_getColumn() > 9) {
      terminal_delchar();
      if (len(command) > 0) pop(command);
    }
    else if (c > 30) {
      terminal_delchar();
      terminal_putchar(c);
      terminal_putchar(219);
      append(command, c);
    }
    if (probe(10+id) == Sender && receive(10+id) == 0xDEAD) {
      printf("plush %d$ ", id-1);
      for (int i = 0; i < len(command); i++) {
        terminal_putchar(get(command, i));
      }
      terminal_putchar(219);
    }
    if (probe(0) == Sender && receive(0) == 0xE5D) {
      return;
    }
    switchToNextTask();
  }

}

void init_shell(void) {
  printf("Shell initialisation : Commands ...");
  initCommands();
  printf(" Done.   Prefix Tree ...");
  makeTree();
  printf(" Done. \n");
}

void shellProcess(void) {
  int id = receive(1);
  terminal_initialize();
  printf("plush %d$ ", id-1);
  flushBuf();
  uint16_t* terBuf = terminalSaveBuffer();
  size_t* cur = terminalSaveCursor();
  list command = empty_list();
  send(1, 0xDEAD);
  while(1) {
    receive(id);
    terminalLoadBuffer(terBuf);
    terminalLoadCursor(cur);
    exec_shell(id, command);
    terBuf = terminalSaveBuffer();
    cur = terminalSaveCursor();
    send(1, 0xDEAD);
  }
}
