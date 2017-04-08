BITS 16
	org 0x7c00

	mov ax, cs
	mov ds, ax

	; read sectors (0x1000:0x0)
	mov ax, 0x1000
	mov es, ax
	mov bx, 0x0

	mov ah, 0x2	; read
	mov al, 0x3	; sectors
	mov ch, 0x0	; cylinder
	mov cl, 0x2	; sector
	mov dh, 0x0	; head
	mov dl, 0x0	; drive
	int 0x13

	; gdt
	lgdt [gdtr]

	; A20 gate
	in al, 0x92
	or al, 2
	out 0x92, al

	; Protected mode
	mov eax, cr0
	or al, 1
	mov cr0, eax

	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov sp, 0x1000
	jmp dword 0x8:0x10000

align 4
gdtr:
	dw gdt_end - gdt -1
	dd gdt
gdt:
	dq 0

	; code
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x88
	db 0x42
	db 0x00

	; data
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x82
	db 0x42
	db 0x00
gdt_end:

times	0x1fe-($-$$) db 0
	dw 0xaa55
