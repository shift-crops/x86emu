global put_esmem

BITS 16
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
