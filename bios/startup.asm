extern bios_main
global start, put_esmem, bs_test

BITS 16
start:
	mov ax, cs
	mov ds, ax
	mov ss, ax
	mov sp, 0x2000

	xor ax, ax
	mov es, ax

	call dword bios_main
	jmp 0x0:0x7c00

put_esmem:
	push ebp
	mov ebp, esp
	push edi
	mov di, word [ebp+0x8]
	mov ax, word [ebp+0xc]
	mov word [es:di], ax
	pop edi
	o32 leave
	o32 ret

bs_test:
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
