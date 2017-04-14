global _puts, _putc, _gets
global sys_puts, sys_gets, irq_timer, irq_keyboard, irq_mouse

extern print_key

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

sys_puts:
	pusha
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
	push ax
	in al, 0x60
	push ax
	call print_key
	add esp, 2
	pop ax
	iret

irq_mouse:
	push ax
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
