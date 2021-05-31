INCLUDEDIR=kernel/include
LIBDIR=kernel/lib
BUILDDIR=_build

CC=i686-elf-gcc
AS=i686-elf-as
ARCH=i386

CFLAGS=--sysroot=sysroot/ -isystem=/usr/include -ffreestanding -O2 -Wall -Wextra -mgeneral-regs-only -g -I$(INCLUDEDIR) -I$(LIBDIR)/include
LDFLAGS=--sysroot=sysroot/ -isystem=/usr/include -ffreestanding -nostdlib -lgcc -g
ASFLAGS=-g

LINKER=linker.ld

NAME=femur
KERNEL=sysroot/boot/$(NAME).kernel
ISO=$(NAME).iso

_HEADERS = cpu_init.h io.h keyboard.h multiboot.h shell.h terminal.h time.h paging.h kernel.h screen.h
HEADERS = $(patsubst %,$(INCLUDEDIR)/%,$(_HEADERS))

_LIBS = kmalloc.h list.h printf.h bitset.h random.h
LIBS = $(patsubst %,$(LIBDIR)/include/%,$(_LIBS))

_OBJ = boot.o kmalloc.o list.o printf.o kernel.o terminal.o time.o cpu_init.o isr.o keyboard.o shell.o paging.o bitset.o multitasking.o screen.o random.o
OBJ = $(patsubst %,$(BUILDDIR)/%,$(_OBJ))

all: $(ISO)

$(BUILDDIR)/:
	mkdir -p $(BUILDDIR)/
	mkdir -p sysroot/usr/include
	mkdir -p sysroot/usr/lib
	mkdir -p sysroot/boot

$(BUILDDIR)/%.o: kernel/arch/$(ARCH)/%.c $(HEADERS) $(LIBS) $(BUILDDIR)/
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/%.o: $(LIBDIR)/src/%.c $(HEADERS) $(LIBS) $(BUILDDIR)/
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/%.o: kernel/modules/%.c $(HEADERS) $(LIBS) $(BUILDDIR)/
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/%.o: kernel/kernel/%.c $(HEADERS) $(LIBS) $(BUILDDIR)/
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/%.o: kernel/arch/$(ARCH)/%.s $(BUILDDIR)/
	$(AS) -c -o $@ $< $(ASFLAGS)

$(BUILDDIR)/%.o: kernel/kernel/%.s $(BUILDDIR)/
		$(AS) -c -o $@ $< $(ASFLAGS)

$(KERNEL): $(OBJ)
	$(CC) -T $(LINKER) -o $@ $^ $(LDFLAGS)

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $< isodir/boot/$(NAME).kernel
	@echo "menuentry \"$(NAME)\" { \n \t multiboot /boot/$(NAME).kernel \n }" > isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

.PHONY: clean launch test
clean:
		rm -f -r isodir
		rm -f -r sysroot
		rm -f -r _build
		rm -f *.iso
launch: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

test: $(ISO)
	qemu-system-i386 -s -S -cdrom $(ISO)
