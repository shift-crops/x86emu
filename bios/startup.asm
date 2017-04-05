extern bios_main, bios_init, fdd_configure
global start

BITS 16
start:
	mov ax, cs
	mov ds, ax
	mov ax, 0x400
	mov ss, ax
	mov sp, 0x2000

	xor ax, ax
	mov es, ax

	call init_pic
	call init_fdd
	call dword bios_init
	call dword bios_main
	call load_mbr
	jmp 0x0:0x7c00

init_pic:
	cli
	mov al, 0x11
	out 0x20, al
	out 0xa0, al

	mov al, 0x20
	out 0x21, al
	mov al, 0x28
	out 0xa1, al

	mov al, 0x4
	out 0x21, al
	mov al, 0x2
	out 0xa1, al

	mov al, 0x1
	out 0x21, al
	out 0xa1, al

	mov al, 0xfb
	out 0x21, al
	mov al, 0xff
	out 0xa1, al
	sti

	ret

init_fdd:
	push 0x0000
	push 0x4000
	call fdd_configure
	add sp, 4

	cli
	in al, 0x21
	and al, 0xbf
	out 0x21, al
	sti
	ret

load_mbr:
	mov ax, 0x0
	mov es, ax
	mov bx, 0x7c00

	mov ah, 0x2	; read
	mov al, 0x1	; sectors
	mov ch, 0x0	; cylinder
	mov cl, 0x1	; sector
	mov dh, 0x0	; head
	mov dl, 0x0	; drive
	int 0x13
	ret
