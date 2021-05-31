#include "terminal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kmalloc.h"

//ce fichier se base sur des fonctions d'osdev un peu modifiées.

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(WHITE, BLACK);
  terminal_buffer = (uint16_t*) 0xB8000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
  }
 }
}

uint16_t * terminalSaveBuffer(void) {
  uint16_t *copy = kmalloc(2*VGA_WIDTH*VGA_HEIGHT);
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      copy[index] = terminal_buffer[index];
    }
   }
  return copy;
}
size_t* terminalSaveCursor(void) {
  size_t* copy = kmalloc(2*sizeof(size_t));
  copy[0] = terminal_row;
  copy[1] = terminal_column;
  return copy;
}
void terminalLoadCursor(size_t* copy) {
  terminal_row = copy[0];
  terminal_column = copy[1];
  kfree(copy);
}
void terminalLoadBuffer(uint16_t * copy) {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = copy[index];
    }
   }
   kfree(copy);
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
      terminal_row = 0;
    }
  }
}
void terminal_delchar(void){
  if (terminal_column == 0) {
    if (terminal_row > 0) {
      terminal_row--;
      terminal_column = VGA_WIDTH-1;
    }
  }
  else terminal_column--;
  terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}
void terminal_changechar(int x, int y, char c) {
  terminal_putentryat(c, terminal_color, x, y);
}

int terminal_getRow(void){
  return terminal_row;
}

int terminal_getColumn(void){
  return terminal_column;
}

void terminal_newline(void) {
  if (++terminal_row == VGA_HEIGHT) {
    terminal_row = 0;
  }
  terminal_column = 0;
}

void terminal_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (data[i] == 8) terminal_delchar();
    else {
      if (data[i] == '\n') terminal_newline();
      else terminal_putchar(data[i]);
  }
}
}

void terminal_writestring(const char* data) {
  terminal_write(data, strlen(data));
}
void terminal_delline(void) {
  while (terminal_column > 0) {
    terminal_delchar();
  }
}
