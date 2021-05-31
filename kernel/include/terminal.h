#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
  BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

size_t strlen(const char* str);
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_newline(void);
void terminal_putchar(char c);
void terminal_delchar(void);
void terminal_writestring(const char* data);
void terminal_write(const char* data, size_t size);
void terminal_changechar(int x, int y, char c);
int terminal_getRow(void);
int terminal_getColumn(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_delline(void);
void terminalLoadBuffer(uint16_t* copy);
uint16_t * terminalSaveBuffer(void);
void terminalLoadCursor(size_t* copy);
size_t* terminalSaveCursor(void);
