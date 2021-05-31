#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "shell.h"
#include "multiboot.h"
#include "cpu_init.h"
#include "keyboard.h"
#include "io.h"
#include "printf.h"
#include "kmalloc.h"
#include "list.h"
#include "paging.h"
#include "kernel.h"
#include "screen.h"

int maxId = 0;
bool preventTaskSwitch = true;
struct process* currentProcess;
struct channel_state* channels;
list processesList;

struct process* getCurrentProcess(void) {return currentProcess;}
void setCurrentProcess(struct process* p) {currentProcess = p;}

int init_mem(multiboot_info_t* mbd) {
  int maxMem = 0;
  if (mbd->flags & 1) {
    maxMem = mbd->mem_upper;
  }
  else {
    printf("Memory could not be detected, assuming 10Mib\n");
    maxMem = 10485760;
  }
  return maxMem*1024;
}

void initMultitasking() {
  processesList = empty_list();
  channels = kmalloc(num_channels * sizeof(struct channel_state));
  currentProcess = kmalloc(sizeof(struct process));
  currentProcess->parentId = 0;
  currentProcess->id = 0;
  currentProcess->state = Running;
  currentProcess->slicesLeft = max_time_slices;
  currentProcess->priority = 0;
  currentProcess->stackTop = 0;
  currentProcess->adressSpace = (void*) getCurrentDir();
  currentProcess->next = currentProcess;
  currentProcess->prev = currentProcess;
  append(processesList, (int) currentProcess);
}

void initChannels(void) {
  channels = kmalloc(num_channels * sizeof(struct channel_state));
  for (int i = 0; i < num_channels; i++) {
    channels[i].state = Unused;
  }
}

void initKernel(int maxMem , void* heapBottom) {
  terminal_initialize();
  initKmalloc(heapBottom); //let a tenth of memory for os allocation
  printf("The system has %d KiB of memory.\n", maxMem/1024);
  PIC_remap(0x20, 0x28);
  initPicMasks();
  printf("PIC initialised.\n");
  initPIT();
  printf("PIT initialised.\n");
  if (initPs2()) {
    printf("Ps2 controller initialised\n");
  }
  else {
    printf("Ps2 controller initialization failed\n");
  }
  if (initKeyboard()) {
    printf("Keyboard initialised\n");
  }
  else {
    printf("Keyboard initialization failed\n");
  }
  initPaging(maxMem);
  initMultitasking();
  initChannels();
  printf("Coucou je suis un Kernel!\n");
}

void insertProcess(struct process* process) {
  process->next = currentProcess->next;
  process->prev = currentProcess;
  currentProcess->next->prev = process;
  currentProcess->next = process;
}

void delCurrentProcess(void) {
  preventTaskSwitch = true;
  currentProcess->next->prev = currentProcess->prev;
  currentProcess->prev->next = currentProcess->next;
  preventTaskSwitch = false;
}

struct process* newProcess(void (*function)(void), int parentId, int priority, bool createNewAdressSpace, bool isUser) {
    extern void initTask(void (*function)(void), struct process* currentTask, struct process* taskToInit);
    struct process *task = kmalloc(sizeof(struct process));
    task->parentId = parentId;
    task->id = ++maxId;
    task->state = Runnable;
    task->slicesLeft = max_time_slices;
    task->priority = priority;
    task->stackTop = (void*) 0xFF000000;
    if (createNewAdressSpace) task->adressSpace = (void*) newPageDirectory(isUser);
    else task->adressSpace = (void*) getCurrentDir();
    initTask(function, currentProcess, task);
    append(processesList, (int) task);
    insertProcess(task);
    return task;
}

void switchTaskWrapper(struct process* nextTask) {
  terminal_putentryat((char) nextTask->id + 48, CYAN, 78, 24);
  extern void switchTask(struct process* currentTask, struct process* nextTask);
  struct process* prevProcess = currentProcess;
  currentProcess = nextTask;
  asm("cli"); //we don't want a switch task interrupt to pop inside another if the scheduling takes too long or whatever
  sendEOI(0); //sometimes we will send EOI when it is not needed (for channel operations for example). In that case it is ignored by the PIC.
  switchTask(prevProcess, nextTask);
}

void switchToNextTask(void) {
  switchTaskWrapper(currentProcess->next);
}

int send(int id, int value) {
  switch (channels[id].state) {

    case Unused: ; //si le canal est inutilisé, le processus est placé en attente
      channels[id].state = Sender;
      channels[id].send.id = currentProcess->id;
      channels[id].send.value = value;
      currentProcess->state = BlockedWriting;
      delCurrentProcess();
      switchToNextTask();
      return 0;

    case Receiver: ; //si il recoit, le processus qui recevait est réveillé et il est exécuté ensuite
      channels[id].send.value = value;
      int pid = pop(channels[id].receive);
      ((struct process*) get(processesList, pid))->state = Runnable;
      insertProcess((struct process*) get(processesList, pid));
      if (len(channels[id].receive) == 0) {
        channels[id].state = Unused;
        delList(channels[id].receive);
      }
      switchToNextTask();
      return 0;

    default: ;
      return -1;
  }
}

int receive(int id) {
  switch (channels[id].state) {

    case Unused: ; //le processus est placé en attente
      channels[id].state = Receiver;
      channels[id].receive = empty_list();
      append(channels[id].receive, currentProcess->id);
      currentProcess->state = BlockedReading;
      delCurrentProcess();
      switchToNextTask();
      return channels[id].send.value;

    case Sender: ; //le processus qui attendais est révéillé, mais il ne récupère par la main : on laisse d'abord le receveur traiter la valeur qu'il a recu.
      channels[id].state = Unused;
      int pid = channels[id].send.id;
      ((struct process*) get(processesList, pid))->state = Runnable;
      insertProcess((struct process*) get(processesList, pid));
      return channels[id].send.value;

    case Receiver: ; //on ajoute le processus à la liste des receveurs sur ce canal.
      append(channels[id].receive, currentProcess->id);
      currentProcess->state = BlockedReading;
      delCurrentProcess();
      switchToNextTask();
      return channels[id].send.value;

    default: ;
      return -1;
  }
}

enum channel_state_enum probe(int id) { //permet de voir l'état d'un canal
  return channels[id].state;
}

void exit(void) {
  preventTaskSwitch = true;
  delCurrentProcess();
  //TODO : libérer la mémoire.
  preventTaskSwitch = false;
  switchToNextTask();
}

void onTimerInterrupt(void) {
  if (--currentProcess->slicesLeft <= 0 && !preventTaskSwitch) {
    currentProcess->slicesLeft = max_time_slices;
    switchToNextTask(); //this functions sends EOI to the PIC just before switching, after disabling interrupts.
  }
  else sendEOI(0);
}

void kernel_main(int maxMem , void* heapBottom) {
  initKernel(maxMem, heapBottom);
  freeze();
  preventTaskSwitch = false;
  init_shell();
  for (int i = 1; i < 10; i++) { //processus pour les différents shell
    newProcess(shellProcess, 0, 1, 1, 0);
    send(1, i+1);
    receive(1);
  }
  newProcess(screenProcess, 0, 1, 1, 0); //processus pour les jeux
  int currentId;
  send(2, 0);
  currentId = 2;
  while(1) {
    asm("hlt");
    switchToNextTask();
    if (getKey(LCTRL) && getKey(LSHIFT)) {
      if (getKey(ESPERLUETTE) && currentId != 2) {
        currentId = 2;
        send(0, 0xE5D); //arrête le shell en cours d'execution
        receive(1); //attend de recevoir un message de sa part (lui laisse le temps de sauvegarder etc)
        send(2, 0); //active le shell 2
      }
      else if (getKey(E_AIGU) && currentId != 3) {
        currentId = 3;
        send(0, 0xE5D);
        receive(1);
        send(3, 0);
      }
      else if (getKey(GUILLEMETS) && currentId != 4) {
        currentId = 4;
        send(0, 0xE5D);
        receive(1);
        send(4, 0);
      }
      else if (getKey(APOSROPHE) && currentId != 5) {
        currentId = 5;
        send(0, 0xE5D);
        receive(1);
        send(5, 0);
      }
      else if (getKey(LPAR) && currentId != 6) {
        currentId = 6;
        send(0, 0xE5D);
        receive(1);
        send(6, 0);
      }
      else if (getKey(TIRET) && currentId != 7) {
        currentId = 7;
        send(0, 0xE5D);
        receive(1);
        send(7, 0);
      }
      else if (getKey(E_GRAVE) && currentId != 8) {
        currentId = 8;
        send(0, 0xE5D);
        receive(1);
        send(8, 0);
      }
      else if (getKey(UNDERSCORE) && currentId != 9) {
        currentId = 9;
        send(0, 0xE5D);
        receive(1);
        send(9, 0);
      }
      else if (getKey(C_CEDILLE) && currentId != 10) {
        currentId = 10;
        send(0, 0xE5D);
        receive(1);
        send(10, 0);
      }
      else if (getKey(A_ACCENT) && currentId != 11) {
        currentId = 11;
        send(0, 0xE5D);
        receive(1);
        send(43, 0);
      }
    }
  }
}
