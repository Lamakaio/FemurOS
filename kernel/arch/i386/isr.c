#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "io.h"
#include "terminal.h"
#include "time.h"
#include "printf.h"
#include "keyboard.h"
#include "paging.h"
#include "kernel.h"

//Les interruptions intéressantes sont 14 (PageFault, qui réalloue une page), 31 et 32 (timer et clavier)

void sendEOI(int picNumber) { //picNumber is 0 for master and 1 for slave
  if(picNumber)
		outb(PIC2_COMMAND,PIC_EOI);
	outb(PIC1_COMMAND,PIC_EOI);
}
void isrnull(void){}
void isr0(void)
 {
 printf("\n Interrupt %d \n",0);

}
void isr1(void)
 {
 printf("\n Interrupt %d \n",1);

}
void isr2(void)
 {
 printf("\n Interrupt %d \n",2);

}
void isr3(void)
 {
 printf("\n Interrupt %d \n",3);

}
void isr4(void)
 {
 printf("\n Interrupt %d \n",4);

}
void isr5(void)
 {
 printf("\n Interrupt %d \n",5);

}
void isr6(void)
 {
 printf("\n Interrupt %d \n",6);

}
void isr7(void)
 {
 printf("\n Interrupt %d \n",7);

}
void isr8(void)
 {
 printf("\n Interrupt %d \n",8);
 freeze();

}
void isr9(void)
 {
 printf("\n Interrupt %d \n",9);

}
void isr10(void)
 {
 printf("\n Interrupt %d \n",10);

}
void isr11(void)
 {
 printf("\n Interrupt %d \n",11);

}
void isr12(void)
 {
 printf("\n Interrupt %d \n",12);

}
void isr13(int errorcode)
 {
 printf("\n Interrupt %d %b \n",13, errorcode);
 freeze();
}
void isr14(int errorcode)
 {
 uint32_t faulting_address;
 asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
 printf("\n Interrupt %d %h %b \n", 14, faulting_address, errorcode, *((int *)(((PageDirectory) getCurrentDir())[faulting_address>>22].pageAdress << 12)));
 alloc_page(faulting_address, (PageDirectory) getCurrentDir());
}
void isr15(void)
 {
 printf("\n Interrupt %d \n",15);

}
void isr16(void)
 {
 printf("\n Interrupt %d \n",16);

}
void isr17(void)
 {
 printf("\n Interrupt %d \n",17);

}
void isr18(void)
 {
 printf("\n Interrupt %d \n",18);

}
void isr19(void)
 {
 printf("\n Interrupt %d \n",19);

}
void isr20(void)
 {
 printf("\n Interrupt %d \n",20);

}
void isr21(void)
 {
 printf("\n Interrupt %d \n",21);

}
void isr22(void)
 {
 printf("\n Interrupt %d \n",22);

}
void isr23(void)
 {
 printf("\n Interrupt %d \n",23);

}
void isr24(void)
 {
 printf("\n Interrupt %d \n",24);

}
void isr25(void)
 {
 printf("\n Interrupt %d \n",25);

}
void isr26(void)
 {
 printf("\n Interrupt %d \n",26);

}
void isr27(void)
 {
 printf("\n Interrupt %d \n",27);

}
void isr28(void)
 {
 printf("\n Interrupt %d \n",28);

}
void isr29(void)
 {
 printf("\n Interrupt %d \n",29);

}
void isr30(void)
 {
 printf("\n Interrupt %d \n",30);

}
void isr31(void)
 {
 printf("\n Interrupt %d \n",31);

}
void isr32(void)
{
 incTime();
 onTimerInterrupt();
}
void isr33(void)
 {
 uint8_t scancode1 = inb(0x60);
 uint8_t scancode2 = inb(0x60);
 uint8_t scancode3 = inb(0x60);
translate(scancode1, scancode2, scancode3);
sendEOI(0);
}
void isr34(void)
 {
 printf("\n Interrupt %d \n",34);
 sendEOI(0);
}
void isr35(void)
 {
 printf("\n Interrupt %d \n",35);
 sendEOI(0);
}
void isr36(void)
 {
 printf("\n Interrupt %d \n",36);
 sendEOI(0);
}
void isr37(void)
 {
 printf("\n Interrupt %d \n",37);
 sendEOI(0);
}
void isr38(void)
 {
 printf("\n Interrupt %d \n",38);
 sendEOI(0);
}
void isr39(void)
 {
 printf("\n Interrupt %d \n",39);
 sendEOI(0);
}
void isr40(void)
 {
 printf("\n Interrupt %d \n",40);
 sendEOI(1);
}
void isr41(void)
 {
 printf("\n Interrupt %d \n",41);
 sendEOI(1);
}
void isr42(void)
 {
 printf("\n Interrupt %d \n",42);
 sendEOI(1);
}
void isr43(void)
 {
 printf("\n Interrupt %d \n",43);
 sendEOI(1);
}
void isr44(void)
 {
 printf("\n Interrupt %d \n",44);
 sendEOI(1);
}
void isr45(void)
 {
 printf("\n Interrupt %d \n",45);
 sendEOI(1);
}
void isr46(void)
 {
 printf("\n Interrupt %d \n",46);
 sendEOI(1);
}
void isr47(void)
 {
 printf("\n Interrupt %d \n",47);
 sendEOI(1);
}
void isr48(void)
 {
 printf("\n Interrupt %d \n",48);
}
void isr49(void)
 {
 printf("\n Interrupt %d \n",49);
}
