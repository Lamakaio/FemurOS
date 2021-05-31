#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cpu_init.h"
#include "io.h"
#include "terminal.h"


//************code de osdev -> l93******************
#define TYPE_INTERRUPT_GATE_USER 0b10001110
#define TYPE_INTERRUPT_GATE_KERNEL 0b11101110
#define TYPE_TRAP_GATE_USER 0b10001111
#define TYPE_TRAP_GATE_KERNEL 0b11101111

//ps2 commands
#define DISABLE_FIRST_PS2 0xAD
#define DISABLE_SECOND_PS2 0xA7
#define ENABLE_FIRST_PS2 0xAE
#define ENABLE_SECOND_PS2 0xA8
#define SET_CONFIG_PS2 0x60
#define READ_CONFIG_PS2 0x20
#define INIT_PS2 0xAA
//ps2 answers
#define ANSWER_CLEAR_PS2 0x55
//ps2 ports
#define PS2_CONTROLLER_COMMAND_PORT 0x64
#define PS2_CONTROLLER_STATUS_PORT 0x64
#define PS2_CONTROLLER_DATA_PORT 0x60

/*code de osdev*/
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#ifndef PIC1_COMMAND
	#define PIC1_COMMAND	PIC1
#endif
#define PIC1_DATA	(PIC1+1)
#ifndef PIC2_COMMAND
	#define PIC2_COMMAND	PIC2
#endif
#define PIC2_DATA	(PIC2+1)


#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */


/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;

	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

//***********************fin du code de osdev*********************

void initPicMasks(void) {
	outb(PIC2_DATA, 0xff);
	outb(PIC1_DATA, ~(0x03));
}

void initPIT(void) { //init the PIT at roughly 100Hz (99,9985Hz)
	asm("cli");
	outb(0x40, 0x9C);
	outb(0x40, 0x2E);
	asm("sti");
}
uint8_t receivePs2Answer(void) { //attend une réponse du Ps2
	int i = 100; //only perform 100 checks
	while (i-- > 0) {
		uint8_t status = inb(PS2_CONTROLLER_STATUS_PORT);
		if (status &	1) {
			return inb(PS2_CONTROLLER_DATA_PORT);
		}
	}
	return 0;
}

int initPs2(void) { //initialise le Ps2
	outb(PS2_CONTROLLER_COMMAND_PORT, DISABLE_FIRST_PS2);
	io_wait();
	outb(PS2_CONTROLLER_COMMAND_PORT, DISABLE_SECOND_PS2);
	io_wait();
	//flush the data port
	inb(PS2_CONTROLLER_DATA_PORT);
	io_wait();
	//send the config to the PS2 controller
	outb(PS2_CONTROLLER_COMMAND_PORT, SET_CONFIG_PS2);
	io_wait();
	outb(PS2_CONTROLLER_DATA_PORT, 0b00000100);
	io_wait();
	//Perform self-test
	outb(PS2_CONTROLLER_COMMAND_PORT, INIT_PS2);
	if (receivePs2Answer() == ANSWER_CLEAR_PS2) {
		outb(PS2_CONTROLLER_COMMAND_PORT, ENABLE_FIRST_PS2);
		return 1;
	}
	else {
		return 0;
	}
}
