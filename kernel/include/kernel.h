#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "list.h"

#define max_time_slices 5
#define max_priority 16
#define num_processes 32
#define num_channels 128
#define num_registers 5

enum process_state_enum {
  Running, Free, BlockedWriting, BlockedReading, Waiting, Runnable, Zombie
};

struct process { //ne pas réarranger
  void* stackTop;
  void* adressSpace;
  void* heapTop;
  void* heapMaxVadress;
  struct process* next;
  struct process* prev;
  enum process_state_enum state;
  char parentId;
  char id;
  int slicesLeft;
  char priority;
};

struct channel_state_sender {
  int id;
  int value;
};

enum channel_state_enum {
  Unused, Sender, Receiver
};

struct channel_state {
  enum channel_state_enum state;
  struct channel_state_sender send;
  list receive;
};


struct process* getCurrentProcess(void);
void setCurrentProcess(struct process* p);
void onTimerInterrupt(void);
int send(int id, int value);
int receive(int id);
void exit(void);
enum channel_state_enum probe(int id);
struct process* newProcess(void (*function)(void), int parentId, int priority, bool createNewAdressSpace, bool isUser);
void switchToNextTask(void);
void switchTaskWrapper(struct process* nextTask);
