	.set ALIGN, 1<<0
	.set MEMINFO, 1<<1
	.set FLAGS, ALIGN | MEMINFO
	.set MAGIC, 0x1BADB002 /*nombre magique qui permet au bootloader de trouver l'entête*/
	.set CHECKSUM, -(MAGIC + FLAGS)

	#valeurs standards pour déclarer le programme comme kernel*/
	.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

	#création dun stack
	.section .bss
	.align 16
stack_bottom:
heap_bottom:
	.skip 0x500000 /* Kernel stack + heap, 3MiB. The kernel will use this memory before multitasking is enabled.
	Kernel processes will have their own stack.
	Kernel should fit in two page entry, making the first 8 MiB of memory kernel reserved*/
stack_top:

  .section .text
.p2align 4
idt:	#idt pour 50 interruptions
    .skip 50*8

idtr: #structure à donner à lidt
    .short (50*8)-1
    .int idt

	.p2align 4
gdt:	#le gdt
	gdt_null: #descripteur nul
		.short 0
		.short 0

		.byte 0
		.byte 0
		.byte 0
		.byte 0

	gdt_code: #descripteur du code, sur toute la mémoire
		.short 0x0ffff
		.short 0

		.byte 0
		.byte 0b10011010
		.byte 0b11001111
		.byte 0

	gdt_data:
		.short 0x0ffff
		.short 0

		.byte 0
		.byte 0b10010010
		.byte 0b11001111
		.byte 0
	gdt_end:

gdtr:
	.short gdt_end - gdt
	.int gdt

	int0_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr0
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int1_handler:
			 push %edx
			 push %ecx
			 push %eax
 	 	 	 call isr1
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int2_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr2
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int3_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr3
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int4_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr4
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int5_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr5
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int6_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr6
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int7_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr7
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int8_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr8
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int9_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr9
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int10_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr10
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int11_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr11
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int12_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr12
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int13_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr13
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int14_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr14
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int15_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr15
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int16_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr16
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int17_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr17
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int18_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr18
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int19_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr19
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int20_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr20
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int21_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr21
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int22_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr22
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int23_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr23
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int24_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr24
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int25_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr25
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int26_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr26
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int27_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr27
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int28_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr28
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int29_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr29
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int30_handler:
			 pop %ebx
			 push %edx
			 push %ecx
			 push %eax
			 push %ebx
	 	 	 call isr30
			 pop %ebx
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int31_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr31
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int32_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr32
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int33_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr33
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int34_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr34
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int35_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr35
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int36_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr36
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int37_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr37
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int38_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr38
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int39_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr39
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int40_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr40
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int41_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr41
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int42_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr42
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int43_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr43
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int44_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr44
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int45_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr45
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int46_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr46
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int47_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr47
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int48_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr48
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret
	int49_handler:
			 push %edx
			 push %ecx
			 push %eax
	 	 	 call isr49
	 	 	 pop %eax
			 pop %ecx
			 pop %edx
			 iret

	.globl encode_isr
	.type encode_idr, @function
encode_isr:
			shl $3, %edx
			add $idt, %edx
			mov %ax, (%edx)
			movw $0x08, 2(%edx)
			movw $0x8e00, 4(%edx)
			shr $16, %eax
			mov %ax, 6(%edx)
			ret
	.globl set_idt
	.type set_idt, @function
set_idt:
	   lidt (idtr)
	 	 mov $0, %edx
 	 	 mov $int0_handler, %eax
 	 	 call encode_isr
	 	 mov $1, %edx
 	 	 mov $int1_handler, %eax
 	 	 call encode_isr
	 	 mov $2, %edx
 	 	 mov $int2_handler, %eax
 	 	 call encode_isr
	 	 mov $3, %edx
 	 	 mov $int3_handler, %eax
 	 	 call encode_isr
	 	 mov $4, %edx
 	 	 mov $int4_handler, %eax
 	 	 call encode_isr
	 	 mov $5, %edx
 	 	 mov $int5_handler, %eax
 	 	 call encode_isr
	 	 mov $6, %edx
 	 	 mov $int6_handler, %eax
 	 	 call encode_isr
	 	 mov $7, %edx
 	 	 mov $int7_handler, %eax
 	 	 call encode_isr
	 	 mov $8, %edx
 	 	 mov $int8_handler, %eax
 	 	 call encode_isr
	 	 mov $9, %edx
 	 	 mov $int9_handler, %eax
 	 	 call encode_isr
	 	 mov $10, %edx
 	 	 mov $int10_handler, %eax
 	 	 call encode_isr
	 	 mov $11, %edx
 	 	 mov $int11_handler, %eax
 	 	 call encode_isr
	 	 mov $12, %edx
 	 	 mov $int12_handler, %eax
 	 	 call encode_isr
	 	 mov $13, %edx
 	 	 mov $int13_handler, %eax
 	 	 call encode_isr
	 	 mov $14, %edx
 	 	 mov $int14_handler, %eax
 	 	 call encode_isr
	 	 mov $15, %edx
 	 	 mov $int15_handler, %eax
 	 	 call encode_isr
	 	 mov $16, %edx
 	 	 mov $int16_handler, %eax
 	 	 call encode_isr
	 	 mov $17, %edx
 	 	 mov $int17_handler, %eax
 	 	 call encode_isr
	 	 mov $18, %edx
 	 	 mov $int18_handler, %eax
 	 	 call encode_isr
	 	 mov $19, %edx
 	 	 mov $int19_handler, %eax
 	 	 call encode_isr
	 	 mov $20, %edx
 	 	 mov $int20_handler, %eax
 	 	 call encode_isr
	 	 mov $21, %edx
 	 	 mov $int21_handler, %eax
 	 	 call encode_isr
	 	 mov $22, %edx
 	 	 mov $int22_handler, %eax
 	 	 call encode_isr
	 	 mov $23, %edx
 	 	 mov $int23_handler, %eax
 	 	 call encode_isr
	 	 mov $24, %edx
 	 	 mov $int24_handler, %eax
 	 	 call encode_isr
	 	 mov $25, %edx
 	 	 mov $int25_handler, %eax
 	 	 call encode_isr
	 	 mov $26, %edx
 	 	 mov $int26_handler, %eax
 	 	 call encode_isr
	 	 mov $27, %edx
 	 	 mov $int27_handler, %eax
 	 	 call encode_isr
	 	 mov $28, %edx
 	 	 mov $int28_handler, %eax
 	 	 call encode_isr
	 	 mov $29, %edx
 	 	 mov $int29_handler, %eax
 	 	 call encode_isr
	 	 mov $30, %edx
 	 	 mov $int30_handler, %eax
 	 	 call encode_isr
	 	 mov $31, %edx
 	 	 mov $int31_handler, %eax
 	 	 call encode_isr
	 	 mov $32, %edx
 	 	 mov $int32_handler, %eax
 	 	 call encode_isr
	 	 mov $33, %edx
 	 	 mov $int33_handler, %eax
 	 	 call encode_isr
	 	 mov $34, %edx
 	 	 mov $int34_handler, %eax
 	 	 call encode_isr
	 	 mov $35, %edx
 	 	 mov $int35_handler, %eax
 	 	 call encode_isr
	 	 mov $36, %edx
 	 	 mov $int36_handler, %eax
 	 	 call encode_isr
	 	 mov $37, %edx
 	 	 mov $int37_handler, %eax
 	 	 call encode_isr
	 	 mov $38, %edx
 	 	 mov $int38_handler, %eax
 	 	 call encode_isr
	 	 mov $39, %edx
 	 	 mov $int39_handler, %eax
 	 	 call encode_isr
	 	 mov $40, %edx
 	 	 mov $int40_handler, %eax
 	 	 call encode_isr
	 	 mov $41, %edx
 	 	 mov $int41_handler, %eax
 	 	 call encode_isr
	 	 mov $42, %edx
 	 	 mov $int42_handler, %eax
 	 	 call encode_isr
	 	 mov $43, %edx
 	 	 mov $int43_handler, %eax
 	 	 call encode_isr
	 	 mov $44, %edx
 	 	 mov $int44_handler, %eax
 	 	 call encode_isr
	 	 mov $45, %edx
 	 	 mov $int45_handler, %eax
 	 	 call encode_isr
	 	 mov $46, %edx
 	 	 mov $int46_handler, %eax
 	 	 call encode_isr
	 	 mov $47, %edx
 	 	 mov $int47_handler, %eax
 	 	 call encode_isr
	 	 mov $48, %edx
 	 	 mov $int48_handler, %eax
 	 	 call encode_isr
	 	 mov $49, %edx
 	 	 mov $int49_handler, %eax
 	 	 call encode_isr
		 ret
			/*
	    mov $int_handler, %eax
	    mov %ax, (idt+49*8)
	    movw $0x08, (idt+49*8+2)
	    movw $0x8e00, (idt+49*8+4)
	    shr $16, %eax
	    mov %ax, (idt+49*8+6)
			ret*/

	.globl reloadSegments
reloadSegments:
			        # Reload CS register containing code selector:
			      ljmp    $0x08,  $reload_CS # 0x08 points at the new code selector
reload_CS:
			        # Reload data segment registers:
			movw    $0x10,          %ax    # 0x10 points at the new data selector
			movw    %ax,            %ds
			movw    %ax,            %es
			movw    %ax,            %fs
			movw    %ax,            %gs
			movw    %ax,            %ss
			ret
	.global _start
	.type _start, @function
_start:
	/*desormais, le programme est chargé en mode 32-bits protégé. */
	cli
	mov $stack_top, %esp

	#initialisation de la GDT
	mov %esp, %ebp
	xor %ax, %ax
	mov %ax, %ds
	lgdt (gdtr)
	call reloadSegments
	call set_idt
	mov %esp, %ebp
	sti
	push %ebx
	call init_mem
	mov %esp, %ebp
	push $heap_bottom
	push %eax
	call kernel_main
	#ne devrait pas revenir ici, donc softlock*/
	cli
1:	hlt
	jmp 1

	.size _start, . - _start
