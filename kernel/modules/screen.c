
#include "kernel.h"
#include "terminal.h"
#include "random.h"
#include "time.h"
#include "keyboard.h"
#include "printf.h"
#include "kmalloc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#define NUMPROGS 1


void * progs[NUMPROGS];
enum dir {
  South,
  East,
  North,
  West
};
struct pos {
  uint8_t x;
  uint8_t y;
};

struct pos spawnApple(void) {
  struct pos ret;
  ret.x = randint(0, VGA_WIDTH);
  ret.y = randint(0, VGA_HEIGHT);
  terminal_putentryat(2, RED, ret.x, ret.y);
  return ret;
}
bool intersect(struct pos obj, struct pos* table, int head, int queue) {
  while(head != queue) {
    if (++queue == VGA_WIDTH*VGA_HEIGHT) queue = 0;
    if (obj.x == table[queue].x && obj.y == table[queue].y) return true;
  }
  return false;
}

void loseScreen(int score) {
  terminal_initialize();
  printf("You lost, your score is %d.", score);
  freeze();
}

void snake(void) {
  receive(43);
  srand(getTime());
  terminal_initialize();
  uint16_t* terBuf = terminalSaveBuffer();
  int maxLen = VGA_WIDTH*VGA_HEIGHT;
  struct pos *pos = kmalloc(maxLen*sizeof(struct pos));
  int head = 2;
  int queue = 0;
  struct pos apple;
  enum dir facing = East;
  pos[2].x = 10;
  pos[1].x = 10;
  pos[0].x = 10;
  pos[2].y = 10;
  pos[1].y = 10;
  pos[0].y = 10;
  int speed = 10;
  bool ticked = false;
  freeze();
  do {
    apple = spawnApple();
  } while (intersect(apple, pos, head, queue));
  while(1) {
    if (probe(0) == Sender && receive(0) == 0xE5D) {
      terBuf = terminalSaveBuffer();
      send(1, 0);
      receive(43);
      terminalLoadBuffer(terBuf);
    }
    if (getKey(Z)) {
      facing = North;
    }
    if (getKey(Q)) {
      facing = West;
    }
    if (getKey(S)) {
      facing = South;
    }
    if (getKey(D)) {
      facing = East;
    }
    if (getTime()%speed <= speed/2 && ticked == false) {
      ticked = true;
      int nextHead = (head + 1)%maxLen;
      int nextQueue = (queue + 1)%maxLen;
      switch (facing) {
        case North:
          pos[nextHead].x = pos[head].x;
          pos[nextHead].y = pos[head].y - 1;
          break;
        case East:
          pos[nextHead].x = pos[head].x + 1;
          pos[nextHead].y = pos[head].y;
          break;
        case South:
          pos[nextHead].x = pos[head].x;
          pos[nextHead].y = pos[head].y + 1;
          break;
        case West:
          pos[nextHead].x = pos[head].x - 1;
          pos[nextHead].y = pos[head].y;
          break;
      }
      if (intersect(pos[nextHead], pos, head, nextQueue)
          || pos[nextHead].y >= VGA_HEIGHT
          || pos[nextHead].x >= VGA_WIDTH) {
        loseScreen((head-queue)%maxLen);
        return;
      }
      if (apple.x == pos[nextHead].x && apple.y == pos[nextHead].y) {
        head = nextHead;
        do {
          apple = spawnApple();
        } while (intersect(apple, pos, head, queue));
      }
      else {
        head = nextHead;
        queue = nextQueue;
      }
      terminal_putentryat(219, GREEN, pos[head].x, pos[head].y);
      terminal_putentryat(' ', GREEN, pos[queue].x, pos[queue].y);
    }
    else if (getTime()%speed > speed/2) ticked = false;
    switchToNextTask();
  }
}

void initScreen(void) {
  progs[0] = snake;
}

void screenProcess(void) {
  initScreen();
  while(1) {
    int id = receive(42);
    ((void (*)(void)) progs[id])();
    send(42, 0xDEAD);
  }
}
