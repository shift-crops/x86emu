extern init_idt, init_paging
extern main
global start

BITS 32
start:
	; gdt
	lgdt [gdtr]
	mov eax, 0x28
	ltr ax

	; idt
	cli
	call init_idt
	lidt [eax]
	sti

	; paging
	call init_paging
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	; timer
	cli
	mov al, 0x34
	out 0x43, al
	mov al, 0x9c
	out 0x40, al
	mov al, 0x2e
	out 0x40, al

	in al, 0x21
	and al, 0xfe
	out 0x21, al
	sti

	; keyboard / mouse
	cli
	in al, 0x21
	and al, 0xfd
	out 0x21, al

	in al, 0xa1
	and al, 0xef
	out 0xa1, al
	sti

	mov ax, 0x23
	mov ds, ax
	mov ss, ax
	call 0x1b:main
 	jmp 0x0:0x0

align 8
gdtr:
	dw gdt_end - gdt -1
	dd gdt
gdt:
	dq 0

	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x88
	db 0x42
	db 0x00

	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x82
	db 0x42
	db 0x00

	dw 0x0000
	dw 0x0000
	db 0x00
	db 0xf8
	db 0x42
	db 0x00

	dw 0x0000
	dw 0x0000
	db 0x00
	db 0xf2
	db 0xc2
	db 0x00

	dw 0x0080
	dw tss - start
	db 0x01
	db 0x00
	db 0x00
	db 0x00
gdt_end:

tss:
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dd 0
	dw 0x10
	dw 0
	dd 0x2000
