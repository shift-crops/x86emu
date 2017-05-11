global bsv_disk, bsv_irq_disk, fdd_configure

BITS 16
; bsv_disk
bsv_disk:
	cmp ah, 0x02
	je disk_read_sector
	cmp ah, 0x03
	je disk_write_sector
	;cmp ah, 0x0c
	;je disk_seek_cylinder
	;mov ax, 0x01
	iret

; bsv_irq_disk
bsv_irq_disk:
	cli
	pusha
	mov al, 0x66
	out 0x20, al
	mov dx, 0x03f4
	in al, dx
	test al, 0x80
	jz irq_disk_end
	mov dx, 0x03f5
	test al, 0x40
	jz irq_disk_read
	mov al, [es:bx]
	out  dx, al
	jmp irq_disk_end
irq_disk_read:
	in  al, dx
	mov [es:bx], al
irq_disk_end:
	popa
	inc bx
	iret

; disk_read_sector
disk_read_sector:
	pusha

	mov ah, al
	xchg dh, ch
	xchg ch, cl
read_loop:
	mov di, bx
	add di, 0x200

	push 0xff00
	push 0x1202
	push cx
	push dx
	call fdd_read_data
read_busywait:
	mov dx, 0x03f4
	in al, dx
	test al, 0x80
	jz read_busywait

	mov dx, 0x03f5
read_next:
	in  al, dx
	mov [es:bx], al
	inc bx
	cmp bx, di
	jl read_next

	mov cx, 0
read_status:
	in al, dx
	inc cx
	cmp cx, 7
	jl read_status

	pop dx
	pop cx
	add sp, 0x04
	sub ah, 1
	add ch, 1
	test ah, ah
	jne read_loop

	mov di, sp
	add di, 0x14	
	mov ax, [ss:di]
	and ax, 0xfffe
	mov [ss:di], ax

	popa
	iret

; disk_write_sector
disk_write_sector:
	mov ah, al
	xchg dh, ch
	xchg ch, cl
write_loop:
	mov di, bx
	add di, 0x200

	push 0xff00
	push 0x1202
	push cx
	push dx
	call fdd_write_data
	mov al, [es:bx]
	out  dx, al
	inc bx
write_busywait:
	cmp bx, di
	jl write_busywait

	mov dx, 0x03f5
	mov cx, 0
write_status:
	in al, dx
	inc cx
	cmp cx, 7
	jl write_status

	pop dx
	pop cx
	add sp, 0x04
	sub ah, 1
	add ch, 1
	test ah, ah
	jne write_loop
	iret

; fdd_read_track
fdd_read_track:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x02
	out dx, al
	mov cx, 0x08
	jmp fdd_args

; fdd_write_data
fdd_write_data:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x05
	out dx, al
	mov cx, 0x08
	jmp fdd_args

; fdd_read_data
fdd_read_data:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x06
	out dx, al
	mov cx, 0x08
	jmp fdd_args

; fdd_configure
fdd_configure:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x13
	out dx, al
	mov cx, 0x03
	jmp fdd_args

fdd_args:
	mov si, 0
args_loop:
	cmp si, cx
	jge args_end
	mov al, [bp+si+4]
	out dx, al
	inc si
	jmp args_loop
args_end:
	leave
	ret
