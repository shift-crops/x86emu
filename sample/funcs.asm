global _puts, _putc, _gets
global in_port, out_port, _cli, _sti
global sys_puts, sys_gets, irq_timer, irq_keyboard, irq_mouse

extern print_key, put_text

BITS 32
_puts:
	mov esi, [esp+0x4]
	int 0
	ret
_putc:
	lea esi, [esp+0x4]
	int 0
	ret

_gets:
	mov edi, [esp+0x4]
	int 1
	ret

in_port:
	push ebp
	mov ebp, esp
	push edx
	mov edx, dword [ebp+0x8]
	xor eax, eax
	in al, dx
	pop edx
	leave
	ret

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
	leave
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

sys_puts:
	pusha
	push esi
	call dword put_text
	add esp, 4
	mov edx, 0x03f8
puts_loop:
	mov al, [esi]
	inc esi
	cmp al, 0
	je puts_end
	out dx, al
	jmp puts_loop
puts_end:
	popa
	iret

;sys_puts:
;	pusha
;	mov edx, 0x03f8
;puts_loop:
;	mov al, [esi]
;	inc esi
;	cmp al, 0
;	je puts_end
;	out dx, al
;	jmp puts_loop
;puts_end:
;	popa
;	iret

sys_gets:
	mov edx, 0x03f8
gets_loop:
	in al, dx
	cmp al, 0xa
	je puts_end
	mov [edi], al
	inc edi
	jmp gets_loop
gets_end:
	mov byte [edi], 0x0
	iret

irq_timer:
	mov esi, msg_timer
	jmp sys_puts

irq_keyboard:
	pusha
	mov al, 0x61
	out 0x20, al
	in al, 0x60
	push ax
	call print_key
	add esp, 2
	popa
	iret

irq_mouse:
	push ax
	mov al, 0x64
	out 0xa0, al
	mov al, 0x62
	out 0x20, al
	in al, 0x60
	mov esi, msg_mouse
	pop ax
	jmp sys_puts

msg_timer:
	db "Tick!", 0xa, 0x0
msg_keyboard:
	db "Keyboard Interrupt!", 0xa,0x0
msg_mouse:
	db "Mouse Interrupt!", 0xa, 0x0
