global bsv_test

BITS 16
bsv_test:
	mov dx, 0x03f8
	mov si, msghello
puts_loop:
	mov al, [si]
	inc si
	cmp al, 0
	je puts_end
	out dx, al
	jmp puts_loop
puts_end:
	iret

msghello:
	db "Hello, World", 0x0a, 0x00
