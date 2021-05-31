#include "terminal.h"
#include "list.h"
#include "kmalloc.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

struct strWithLen {
  char * str;
  int len;
};

void printInt(uint32_t n, char base) {
  char chiffres[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char string[32] = {0};
  int i = 31;
  if (n==0) terminal_putchar('0');
  while(n > 0) {
    string[i--] = chiffres[n%base];
    n = n/base;
  }
  i = -1;
  while (++i < 31 && string[i] == 0); //place i au premier caractère non nul, ou au dernier caractère le cas échéant.
  i--; //le place juste avant
  while (++i < 32) terminal_putchar(string[i]); //imprime tout
}


void printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  size_t size = strlen(fmt);
  for (size_t i = 0; i < size; i++) {
    switch (fmt[i]) { //traitement des caractères spéciaux
      case '\n':
      terminal_newline();
      break;
      case '\b':
      terminal_delchar();
      break;
      case '\t':
      terminal_putchar(' ');
      terminal_putchar(' ');
      terminal_putchar(' ');
      break;
      case '\\':
      if (++i >= size) return;
      switch (fmt[i]) {
        case 'n':
        terminal_newline();
        break;
        case 'b':
        terminal_delchar();
        break;
        case 't':
        terminal_putchar(' ');
        terminal_putchar(' ');
        terminal_putchar(' ');
        break;
        case '\\':
        terminal_putchar('\\');
        break;
        case '%':
        terminal_putchar('%');
        break;
        default:
        break;
      }
      break;
      case '%': //traitement des insertions
      if (++i >= size) return;
      switch (fmt[i]) {
        case 'c':
        terminal_putchar(va_arg(args, int));
        break;
        case 'd':
        printInt(va_arg(args, int), 10);
        break;
        case 'b':
        printInt(va_arg(args, int), 2);
        break;
        case 'h':
        printInt(va_arg(args, int), 16);
        break;
        case 'o':
        printInt(va_arg(args, int), 8);
        break;
        case 's':
        terminal_writestring(va_arg(args, char*));
        break;
      }
      break;
      default:
      terminal_putchar(fmt[i]);
      break;
    }
  }
  return;
}
void sprintInt(uint32_t n, char base, list l) {
  char chiffres[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char string[32] = {0};
  int i = 31;
  if (n==0) append(l, '0');
  while(n > 0) {
    string[i--] = chiffres[n%base];
    n = n/base;
  }
  i = -1;
  while (++i < 31 && string[i] == 0); //place i au premier caractère non nul, ou au dernier caractère le cas échéant.
  i--; //le place juste avant
  while (++i < 32) append(l, string[i]); //imprime tout
}

struct strWithLen sprintf(const char* fmt, ...) { //sprintf retourne la string qui serait print avec printf.
  va_list args;
  va_start(args, fmt);
  list l = empty_list();
  size_t size = strlen(fmt);
  for (size_t i = 0; i < size; i++) {
    switch (fmt[i]) {
      case '\n':
      append(l, '\n');
      break;
      case '\b':
      if (len(l) > 0) pop(l);
      break;
      case '\t':
      append(l, ' ');
      append(l, ' ');
      append(l, ' ');
      break;
      case '\\':
      if (++i >= size) break;
      switch (fmt[i]) {
        case 'n':
        append(l, '\n');
        break;
        case 'b':
        if (len(l) > 0) pop(l);
        break;
        case 't':
        append(l, ' ');
        append(l, ' ');
        append(l, ' ');
        break;
        case '\\':
        append(l, '\\');
        break;
        case '%':
        append(l, '%');
        break;
        default:
        break;
      }
      break;
      case '%':
      if (++i >= size) break;
      switch (fmt[i]) {
        case 'c':
        append(l, va_arg(args, int));
        break;
        case 'd':
        sprintInt(va_arg(args, int), 10, l);
        break;
        case 'b':
        sprintInt(va_arg(args, int), 2, l);
        break;
        case 'h':
        sprintInt(va_arg(args, int), 16, l);
        break;
        case 'o':
        sprintInt(va_arg(args, int), 8, l);
        break;
      }
      break;
      default:
      append(l, fmt[i]);
      break;
    }
  }
  char* str = kmalloc(len(l));
  struct strWithLen ret = {str, len(l)};
  for (int i = len(l)-1; i >= 0; i--) {
    str[i] = pop(l);
  }
  delList(l);
  return ret;
}
