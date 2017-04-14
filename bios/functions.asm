global write_esb, write_esw, write_esd
global in_port, out_port, cli, sti

BITS 16
write_esb:
	push ebp
	mov ebp, esp
	push edi
	mov di, word [ebp+0x8]
	mov eax, dword [ebp+0xc]
	mov byte [es:di], al
	pop edi
	o32 leave
	o32 ret

write_esw:
	push ebp
	mov ebp, esp
	push edi
	mov di, word [ebp+0x8]
	mov eax, dword [ebp+0xc]
	mov word [es:di], ax
	pop edi
	o32 leave
	o32 ret

write_esd:
	push ebp
	mov ebp, esp
	push edi
	mov di, word [ebp+0x8]
	mov eax, dword [ebp+0xc]
	mov dword [es:di], eax
	pop edi
	o32 leave
	o32 ret

in_port:
	push ebp
	mov ebp, esp
	push edx
	mov edx, dword [ebp+0x8]
	xor eax, eax
	in al, dx
	pop edx
	o32 leave
	o32 ret

out_port:
	push ebp
	mov ebp, esp
	push eax
	push edx
	mov edx, dword [ebp+0x8]
	mov eax, dword [ebp+0xc]
	out dx, al
	pop edx
	pop eax
	o32 leave
	o32 ret

cli:
	cli
	o32 ret

sti:
	sti
	o32 ret
