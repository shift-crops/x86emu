extern bios_main
global start, put_esmem

BITS 16
start:
	mov ax, cs
	mov ds, ax
	mov ax, 0x400
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
