

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PIC_EOI 0x20
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
/*Send data to io port "port"*/
inline void outb(uint16_t port, uint8_t data) {
  asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

inline void outw(uint16_t port, uint16_t data) {
  asm volatile ("outw %0, %1" : : "r" (data), "r" (port));
}

inline void outl(uint16_t port, uint32_t data) {
  asm volatile ("outl %0, %1"
  :
  : "r" (data), "r" (port) );
}

/*Listen for data on io port "port"*/
inline uint8_t inb(uint16_t port) {
  uint8_t data;
  asm volatile ( "inb %1, %0"
  : "=a" (data)
  : "Nd" (port) );
  return data;
}

inline uint16_t inw(uint16_t port) {
    uint16_t data;
  asm volatile ( "inw %1, %0"
  : "=r" (data)
  : "r" (port) );
  return data;
}

inline uint32_t inl(uint16_t port) {
  uint32_t data;
  asm volatile ( "inl %1, %0"
  : "=r" (data)
  : "r" (port) );
  return data;
}

/*wait for an io operation to complete*/
inline void io_wait(void) {
  /*The port 0x80 seem to be unused according to osdev*/
  asm volatile ("outb %%al, $0x80"
  :
  :"Nd" (0) );
}
void sendEOI(int picNumber);
