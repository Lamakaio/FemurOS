#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "io.h"
#include "terminal.h"
#include "keyboard.h"
#include "cpu_init.h"
#include "time.h"
//ps2 commands
#define DISABLE_FIRST_PS2 0xAD
#define ENABLE_FIRST_PS2 0xAE
#define SET_CONFIG_PS2 0x60
#define READ_CONFIG_PS2 0x20
//ps2 answers
#define ANSWER_CLEAR_PS2 0x55
//ps2 ports
#define PS2_CONTROLLER_COMMAND_PORT 0x64
#define PS2_CONTROLLER_STATUS_PORT 0x64
#define PS2_CONTROLLER_DATA_PORT 0x60

//device commands
#define ENABLE_SCANNING 0xF4
#define DISABLE_SCANNING 0xF5
#define IDENTIFY 0xF2

bool pressedKeys[75] = {false};
int keyTime[74] = {0};
char buffer[256] = {0, 0};

//le driver garde un buffer des touches pressées non lues par un programme, accessible de partout.
//Si une touche particulière est pressée ou non est aussi une information accessible de partout.

inline void writeBuf(void) {
  terminal_write(buffer + 1, buffer[0]);
  buffer[0] = 0;
}
inline void addBuf(char c) {
  buffer[0]++;
  buffer[(int) buffer[0]] = c;
}
inline void delBuf(void) {
  if (buffer[0] > 0) buffer[0]--;
  else addBuf(8);
}
inline void flushBuf(void) {
  buffer[0] = 0;
}
inline char getChar(void) {
  int n = (int) buffer[0];
  buffer[0] = 0;
  if (n) return buffer[n];
  else return 0;
}
inline char *getBuf(void) {
  return buffer;
}
inline void setKey(enum key k) {
  pressedKeys[k] = true;
}

inline void clearKey(enum key k) {
  pressedKeys[k] = false;
}

inline bool getKey(enum key k) {
  return pressedKeys[k];
}

inline bool* getPressedKeys(void) {
  return pressedKeys;
}
int sendToDevice(uint8_t data) {
  int i = 100; //only perform 100 tries
  while (i-- > 0) {
    uint8_t status = inb(PS2_CONTROLLER_STATUS_PORT);
		if (! (status & 2)) {
      int resend = 100;
      while (resend--) {
        	outb(PS2_CONTROLLER_DATA_PORT, data);
          int answer = receivePs2Answer();
          if (answer == 0xFA) {
            return 1;
          }
          else if (answer != 0xFE) {
            return 0;
          }
      }
      return 0;

		}
  }
  return 0;
}

int initKeyboard(void) {
  sendToDevice(DISABLE_SCANNING);
  sendToDevice(IDENTIFY);
  uint8_t a1 = receivePs2Answer();
  uint8_t a2 = receivePs2Answer();
  if (a1 != 0xAB) {
    return 0;
  }
  if (a2 == 0x41 || a2 == 0xC1) {
    terminal_writestring("The keyboard is still in translation mode");
    terminal_newline();
  }
  sendToDevice(ENABLE_SCANNING);
  outb(PS2_CONTROLLER_COMMAND_PORT, SET_CONFIG_PS2);
  outb(PS2_CONTROLLER_DATA_PORT, 0b00000111);
  return 1;
}
void freeze(void) {
  buffer[0] = 0;
  while(buffer[0] == 0) asm("hlt");
}

void write(enum key touche) {
  switch (touche) {
    case ESPERLUETTE:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('1');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('&');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case A:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('A');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('a');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case Z:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('Z');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('z');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case E:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('E');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('e');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case R:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('R');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('r');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case T:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('T');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('t');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case Y:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('Y');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('y');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case U:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('U');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('u');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case I:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('I');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('i');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case O:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('O');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('o');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case P:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('P');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('p');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case Q:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('Q');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('q');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case S:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('S');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('s');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case D:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('D');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('d');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case F:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('F');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('f');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case G:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('G');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('g');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case H:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('H');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('h');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case J:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('J');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('j');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case K:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('K');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('k');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case L:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('L');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('l');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case M:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('M');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('m');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case W:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('W');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('w');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case X:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('X');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('x');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case C:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('C');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('c');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case V:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('V');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('v');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case B:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('B');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('b');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case N:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('N');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('n');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case VIRGULE:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('?');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf(',');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case PVIRGULE:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('.');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf(';');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case _2POINTS:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('/');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf(':');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case EXCLAMATION:
 	 if (getKey(LSHIFT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('$');
 	 	 }
 	 	  else {}
 	}
 	 else {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('!');
 	 	 }
 	 	 else {}
 	}
 	 break;	 case E_AIGU:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('~');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('2');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('e');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case GUILLEMETS:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('#');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('3');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('\"');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case APOSROPHE:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('{');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('4');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('\'');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case LPAR:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('[');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('5');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('(');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case TIRET:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('|');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('6');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('-');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case E_GRAVE:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('`');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('7');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('e');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case UNDERSCORE:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('\\');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('8');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('_');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case C_CEDILLE:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('^');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('9');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('c');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case A_ACCENT:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('@');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('0');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('a');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case RPAR:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf(']');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('.');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf(')');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;	 case EGALE:
 	 if (getKey(LALT)) {
 	 	 if (!getKey(LCTRL)) {
 	 	 	 addBuf('}');
 	 	 }
 	 	 else {}
 	}
 	 else {
 	 	 if (getKey(LSHIFT)) {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('+');
 	 	 	 }
 	 	 	 else {}
 	 	 	}
 	 	 else {
 	 	 	 if (!getKey(LCTRL)) {
 	 	 	 	 addBuf('=');
 	 	 	 }
 	 	 	 else {}
 	 	 }
 	}
 	 break;
   case SPACE:
   if (!getKey(LCTRL)) {
     addBuf(' ');
   }
   else {}
   break;
   case ENTER:
   if (!getKey(LCTRL)) {
     addBuf('\n');
   }
   else {}
   break;
   case DEL:
   if (!getKey(LCTRL)) {
     delBuf();
   }
   else {}
   break;
   default:
   return;
  }
}

void translate(int scancode1, int scancode2, int scancode3) {
  int i = scancode1 + 256 * scancode2 + 65536 * scancode3;
  switch (i) {
  case 1447446:
 	 if (getTime() > keyTime[ESPERLUETTE] + 2) {
 	 setKey(ESPERLUETTE);
 	 write(ESPERLUETTE);
 	 }
 	 break;
 	case 1447664:
 	 clearKey(ESPERLUETTE);
 	 keyTime[ESPERLUETTE] = getTime();
 	 break;
 	case 1973790:
 	 if (getTime() > keyTime[E_AIGU] + 2) {
 	 setKey(E_AIGU);
 	 write(E_AIGU);
 	 }
 	 break;
 	case 1974000:
 	 clearKey(E_AIGU);
 	 keyTime[E_AIGU] = getTime();
 	 break;
 	case 2500134:
 	 if (getTime() > keyTime[GUILLEMETS] + 2) {
 	 setKey(GUILLEMETS);
 	 write(GUILLEMETS);
 	 }
 	 break;
 	case 2500336:
 	 clearKey(GUILLEMETS);
 	 keyTime[GUILLEMETS] = getTime();
 	 break;
 	case 2434341:
 	 if (getTime() > keyTime[APOSROPHE] + 2) {
 	 setKey(APOSROPHE);
 	 write(APOSROPHE);
 	 }
 	 break;
 	case 2434544:
 	 clearKey(APOSROPHE);
 	 keyTime[APOSROPHE] = getTime();
 	 break;
 	case 3026478:
 	 if (getTime() > keyTime[LPAR] + 2) {
 	 setKey(LPAR);
 	 write(LPAR);
 	 }
 	 break;
 	case 3026672:
 	 clearKey(LPAR);
 	 keyTime[LPAR] = getTime();
 	 break;
 	case 3552822:
 	 if (getTime() > keyTime[TIRET] + 2) {
 	 setKey(TIRET);
 	 write(TIRET);
 	 }
 	 break;
 	case 3553008:
 	 clearKey(TIRET);
 	 keyTime[TIRET] = getTime();
 	 break;
 	case 4013373:
 	 if (getTime() > keyTime[E_GRAVE] + 2) {
 	 setKey(E_GRAVE);
 	 write(E_GRAVE);
 	 }
 	 break;
 	case 4013552:
 	 clearKey(E_GRAVE);
 	 keyTime[E_GRAVE] = getTime();
 	 break;
 	case 4079166:
 	 if (getTime() > keyTime[UNDERSCORE] + 2) {
 	 setKey(UNDERSCORE);
 	 write(UNDERSCORE);
 	 }
 	 break;
 	case 4079344:
 	 clearKey(UNDERSCORE);
 	 keyTime[UNDERSCORE] = getTime();
 	 break;
 	case 4605510:
 	 if (getTime() > keyTime[C_CEDILLE] + 2) {
 	 setKey(C_CEDILLE);
 	 write(C_CEDILLE);
 	 }
 	 break;
 	case 4605680:
 	 clearKey(C_CEDILLE);
 	 keyTime[C_CEDILLE] = getTime();
 	 break;
 	case 4539717:
 	 if (getTime() > keyTime[A_ACCENT] + 2) {
 	 setKey(A_ACCENT);
 	 write(A_ACCENT);
 	 }
 	 break;
 	case 4539888:
 	 clearKey(A_ACCENT);
 	 keyTime[A_ACCENT] = getTime();
 	 break;
 	case 5131854:
 	 if (getTime() > keyTime[RPAR] + 2) {
 	 setKey(RPAR);
 	 write(RPAR);
 	 }
 	 break;
 	case 5132016:
 	 clearKey(RPAR);
 	 keyTime[RPAR] = getTime();
 	 break;
 	case 5592405:
 	 if (getTime() > keyTime[EGALE] + 2) {
 	 setKey(EGALE);
 	 write(EGALE);
 	 }
 	 break;
 	case 5592560:
 	 clearKey(EGALE);
 	 keyTime[EGALE] = getTime();
 	 break;
 	case 6710886:
 	 if (getTime() > keyTime[DEL] + 2) {
 	 setKey(DEL);
 	 write(DEL);
 	 }
 	 break;
 	case 6711024:
 	 clearKey(DEL);
 	 keyTime[DEL] = getTime();
 	 break;
 	case 1381653:
 	 if (getTime() > keyTime[A] + 2) {
 	 setKey(A);
 	 write(A);
 	 }
 	 break;
 	case 1381872:
 	 clearKey(A);
 	 keyTime[A] = getTime();
 	 break;
 	case 1907997:
 	 if (getTime() > keyTime[Z] + 2) {
 	 setKey(Z);
 	 write(Z);
 	 }
 	 break;
 	case 1908208:
 	 clearKey(Z);
 	 keyTime[Z] = getTime();
 	 break;
 	case 2368548:
 	 if (getTime() > keyTime[E] + 2) {
 	 setKey(E);
 	 write(E);
 	 }
 	 break;
 	case 2368752:
 	 clearKey(E);
 	 keyTime[E] = getTime();
 	 break;
 	case 2960685:
 	 if (getTime() > keyTime[R] + 2) {
 	 setKey(R);
 	 write(R);
 	 }
 	 break;
 	case 2960880:
 	 clearKey(R);
 	 keyTime[R] = getTime();
 	 break;
 	case 2894892:
 	 if (getTime() > keyTime[T] + 2) {
 	 setKey(T);
 	 write(T);
 	 }
 	 break;
 	case 2895088:
 	 clearKey(T);
 	 keyTime[T] = getTime();
 	 break;
 	case 3487029:
 	 if (getTime() > keyTime[Y] + 2) {
 	 setKey(Y);
 	 write(Y);
 	 }
 	 break;
 	case 3487216:
 	 clearKey(Y);
 	 keyTime[Y] = getTime();
 	 break;
 	case 3947580:
 	 if (getTime() > keyTime[U] + 2) {
 	 setKey(U);
 	 write(U);
 	 }
 	 break;
 	case 3947760:
 	 clearKey(U);
 	 keyTime[U] = getTime();
 	 break;
 	case 4408131:
 	 if (getTime() > keyTime[I] + 2) {
 	 setKey(I);
 	 write(I);
 	 }
 	 break;
 	case 4408304:
 	 clearKey(I);
 	 keyTime[I] = getTime();
 	 break;
 	case 4473924:
 	 if (getTime() > keyTime[O] + 2) {
 	 setKey(O);
 	 write(O);
 	 }
 	 break;
 	case 4474096:
 	 clearKey(O);
 	 keyTime[O] = getTime();
 	 break;
 	case 5066061:
 	 if (getTime() > keyTime[P] + 2) {
 	 setKey(P);
 	 write(P);
 	 }
 	 break;
 	case 5066224:
 	 clearKey(P);
 	 keyTime[P] = getTime();
 	 break;
 	case 1842204:
 	 if (getTime() > keyTime[Q] + 2) {
 	 setKey(Q);
 	 write(Q);
 	 }
 	 break;
 	case 1842416:
 	 clearKey(Q);
 	 keyTime[Q] = getTime();
 	 break;
 	case 1776411:
 	 if (getTime() > keyTime[S] + 2) {
 	 setKey(S);
 	 write(S);
 	 }
 	 break;
 	case 1776624:
 	 clearKey(S);
 	 keyTime[S] = getTime();
 	 break;
 	case 2302755:
 	 if (getTime() > keyTime[D] + 2) {
 	 setKey(D);
 	 write(D);
 	 }
 	 break;
 	case 2302960:
 	 clearKey(D);
 	 keyTime[D] = getTime();
 	 break;
 	case 2829099:
 	 if (getTime() > keyTime[F] + 2) {
 	 setKey(F);
 	 write(F);
 	 }
 	 break;
 	case 2829296:
 	 clearKey(F);
 	 keyTime[F] = getTime();
 	 break;
 	case 3421236:
 	 if (getTime() > keyTime[G] + 2) {
 	 setKey(G);
 	 write(G);
 	 }
 	 break;
 	case 3421424:
 	 clearKey(G);
 	 keyTime[G] = getTime();
 	 break;
 	case 3355443:
 	 if (getTime() > keyTime[H] + 2) {
 	 setKey(H);
 	 write(H);
 	 }
 	 break;
 	case 3355632:
 	 clearKey(H);
 	 keyTime[H] = getTime();
 	 break;
 	case 3881787:
 	 if (getTime() > keyTime[J] + 2) {
 	 setKey(J);
 	 write(J);
 	 }
 	 break;
 	case 3881968:
 	 clearKey(J);
 	 keyTime[J] = getTime();
 	 break;
 	case 4342338:
 	 if (getTime() > keyTime[K] + 2) {
 	 setKey(K);
 	 write(K);
 	 }
 	 break;
 	case 4342512:
 	 clearKey(K);
 	 keyTime[K] = getTime();
 	 break;
 	case 4934475:
 	 if (getTime() > keyTime[L] + 2) {
 	 setKey(L);
 	 write(L);
 	 }
 	 break;
 	case 4934640:
 	 clearKey(L);
 	 keyTime[L] = getTime();
 	 break;
 	case 5000268:
 	 if (getTime() > keyTime[M] + 2) {
 	 setKey(M);
 	 write(M);
 	 }
 	 break;
 	case 5000432:
 	 clearKey(M);
 	 keyTime[M] = getTime();
 	 break;
 	case 1184274:
 	 if (getTime() > keyTime[LSHIFT] + 2) {
 	 setKey(LSHIFT);
 	 write(LSHIFT);
 	 }
 	 break;
 	case 1184496:
 	 clearKey(LSHIFT);
 	 keyTime[LSHIFT] = getTime();
 	 break;
 	case 1710618:
 	 if (getTime() > keyTime[W] + 2) {
 	 setKey(W);
 	 write(W);
 	 }
 	 break;
 	case 1710832:
 	 clearKey(W);
 	 keyTime[W] = getTime();
 	 break;
 	case 2236962:
 	 if (getTime() > keyTime[X] + 2) {
 	 setKey(X);
 	 write(X);
 	 }
 	 break;
 	case 2237168:
 	 clearKey(X);
 	 keyTime[X] = getTime();
 	 break;
 	case 2171169:
 	 if (getTime() > keyTime[C] + 2) {
 	 setKey(C);
 	 write(C);
 	 }
 	 break;
 	case 2171376:
 	 clearKey(C);
 	 keyTime[C] = getTime();
 	 break;
 	case 2763306:
 	 if (getTime() > keyTime[V] + 2) {
 	 setKey(V);
 	 write(V);
 	 }
 	 break;
 	case 2763504:
 	 clearKey(V);
 	 keyTime[V] = getTime();
 	 break;
 	case 3289650:
 	 if (getTime() > keyTime[B] + 2) {
 	 setKey(B);
 	 write(B);
 	 }
 	 break;
 	case 3289840:
 	 clearKey(B);
 	 keyTime[B] = getTime();
 	 break;
 	case 3223857:
 	 if (getTime() > keyTime[N] + 2) {
 	 setKey(N);
 	 write(N);
 	 }
 	 break;
 	case 3224048:
 	 clearKey(N);
 	 keyTime[N] = getTime();
 	 break;
 	case 3815994:
 	 if (getTime() > keyTime[VIRGULE] + 2) {
 	 setKey(VIRGULE);
 	 write(VIRGULE);
 	 }
 	 break;
 	case 3816176:
 	 clearKey(VIRGULE);
 	 keyTime[VIRGULE] = getTime();
 	 break;
 	case 4276545:
 	 if (getTime() > keyTime[PVIRGULE] + 2) {
 	 setKey(PVIRGULE);
 	 write(PVIRGULE);
 	 }
 	 break;
 	case 4276720:
 	 clearKey(PVIRGULE);
 	 keyTime[PVIRGULE] = getTime();
 	 break;
 	case 4802889:
 	 if (getTime() > keyTime[_2POINTS] + 2) {
 	 setKey(_2POINTS);
 	 write(_2POINTS);
 	 }
 	 break;
 	case 4803056:
 	 clearKey(_2POINTS);
 	 keyTime[_2POINTS] = getTime();
 	 break;
 	case 4868682:
 	 if (getTime() > keyTime[EXCLAMATION] + 2) {
 	 setKey(EXCLAMATION);
 	 write(EXCLAMATION);
 	 }
 	 break;
 	case 4868848:
 	 clearKey(EXCLAMATION);
 	 keyTime[EXCLAMATION] = getTime();
 	 break;
 	case 1315860:
 	 if (getTime() > keyTime[LCTRL] + 2) {
 	 setKey(LCTRL);
 	 write(LCTRL);
 	 }
 	 break;
 	case 1316080:
 	 clearKey(LCTRL);
 	 keyTime[LCTRL] = getTime();
 	 break;
 	case 1118481:
 	 if (getTime() > keyTime[LALT] + 2) {
 	 setKey(LALT);
 	 write(LALT);
 	 }
 	 break;
 	case 1118704:
 	 clearKey(LALT);
 	 keyTime[LALT] = getTime();
 	 break;
 	case 2697513:
 	 if (getTime() > keyTime[SPACE] + 2) {
 	 setKey(SPACE);
 	 write(SPACE);
 	 }
 	 break;
 	case 2697712:
 	 clearKey(SPACE);
 	 keyTime[SPACE] = getTime();
 	 break;
 	case 5921370:
 	 if (getTime() > keyTime[ENTER] + 2) {
 	 setKey(ENTER);
 	 write(ENTER);
 	 }
 	 break;
 	case 5921520:
 	 clearKey(ENTER);
 	 keyTime[ENTER] = getTime();
 	 break;
  }
}
