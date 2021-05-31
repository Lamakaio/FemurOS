	.set ALIGN, 1<<0

    .text
		.globl   switchTask
		.p2align 2
		.type    switchTask,%function
switchTask: /*arguments : processus actuel et processus vers lequel switch*/
		cli
    push %ebx
    push %esi
    push %edi
    push %ebp

    mov 20(%esp), %edi #TCB du processus actuel
    mov %esp, (%edi) #sauvegarde du stackTop

    mov 24(%esp), %esi #prochain TCB

    mov (%esi), %esp #changement de stack
    mov 4(%esi), %eax #
    #ajouter esp0 pour tss
    mov %eax, %cr3
		pop %ebp
		pop %edi
		pop %esi
		pop %ebx
		sti
		ret

		.globl   initTask
		.p2align 2
		.type    initTask,%function
initTask: #initialise une tache en configurant sont stack pour que ret envoie au bon endroit.
		cli
		push %ebx
		push %esi
		push %edi
		push %ebp

		mov 24(%esp), %edi #TCB du processus actuel
		mov %esp, (%edi) #sauvegarde du stackTop

		mov 20(%esp), %ebx
		mov 28(%esp), %esi #prochain TCB

		mov (%esi), %esp #changement de stack
		mov 4(%esi), %eax #nouveau VAS
		#ajouter esp0 pour tss
		mov %cr3, %ecx # sauvegarde de l'ancien VAS
		mov %eax, %cr3 #changement de VAS
		push %ebx	#address de pc pour l'instuction ret executée lors du task switch
		push $0		#l'espace en mémoire est alloué par default.
		push $0
		push $0
		push $0 #4 registres seront pop lors d'un changement de contexte avant l'instruction ret
		mov %esp, (%esi) #sauvegarde du nouveau stackTop
		mov %ecx, %cr3 #restauration : on recharge l'ancien VAS
		mov (%edi), %esp # et l'ancien stack stackTop
		pop %ebp 	#on dépile les registres empilés en on retourne
		pop %edi
		pop %esi
		pop %ebx
		sti
		ret
