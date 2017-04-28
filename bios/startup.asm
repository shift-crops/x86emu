extern bios_main, bios_init
extern fdd_configure
extern attr_configure, seq_configure, dac_configure, gc_configure, load_font
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
	call init_vga
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

init_vga:
	cli
	; mor : 0x2
	mov dx, 0x03c2
	mov al, 0x2
	out dx, al
	sti

	call dword load_font

	cli
	call dword attr_configure
	call dword seq_configure
	call dword dac_configure
	call dword gc_configure

	; x : 320
	mov dx, 0x03b4
	mov al, 0x1
	out dx, al
	mov dx, 0x03b5
	mov al, 0x28
	out dx, al

	; y : 200
	mov dx, 0x03b4
	mov al, 0x12
	out dx, al
	mov dx, 0x03b5
	mov al, 0x19
	out dx, al

	; MSL : 8
	mov dx, 0x03b4
	mov al, 0x09
	out dx, al
	mov dx, 0x03b5
	mov al, 0x8-1
	out dx, al
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
