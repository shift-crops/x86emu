BITS 16
	mov ax, cs
	mov ds, ax

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

	jmp 0x1000:0x0

times	0x1fe-($-$$) db 0
	dw 0xaa55
