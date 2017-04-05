extern main, init_paging
global start, _puts, _gets

BITS 16
start:
	mov ax, cs
	mov ds, ax

	lgdt [gdtr]
	lidt [idtr]
	mov eax, 0x28
	ltr ax

	; A20 gate
	in al, 0x92
	or al, 2
	out 0x92, al

	; Protected mode
	mov eax, cr0
	or al, 1
	mov cr0, eax

	mov ax, 0x23
	mov ds, ax
	mov ss, ax
	mov sp, 0x1000
	jmp 0x1b:_start

BITS 32
_start:
	; paging
	call init_paging
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	; timer
	mov al, 0x34
	out 0x43, al
	mov al, 0x9c
	out 0x40, al
	mov al, 0x2e
	out 0x40, al

	mov al, 0xba
	out 0x21, al

	call main
 	hlt

_puts:
	mov esi, [esp+0x4]
	int 0
	ret

_gets:
	mov edi, [esp+0x4]
	int 1
	ret

sys_puts:
	mov edx, 0x03f8
puts_loop:
	mov al, [esi]
	inc esi
	cmp al, 0
	je puts_end
	out dx, al
	jmp puts_loop
puts_end:
	mov al, 0x0a
	out dx, al
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

sys_timer:
	mov esi, msg_timer
	jmp sys_puts

align 4
gdtr:
	dw idt-gdt
	dd gdt+0x10000
idtr:
	dw end-idt
	dd idt+0x10000
gdt:
	dq 0

	dw 0x0000
	dw 0x0000
	dd 0x00418801

	dw 0x0000
	dw 0x0000
	dd 0x00418201

	dw 0x0000
	dw 0x0000
	dd 0x0041f801

	dw 0x0000
	dw 0x0000
	dd 0x00c1f201

	dw 0x0080
	dw tss
	dd 0x00000001
idt:
	dw sys_puts
	dw 0x0008
	dw 0xef00
	dw 0x0000

	dw sys_gets
	dw 0x0008
	dw 0xef00
	dw 0x0000

	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0

	dw sys_timer
	dw 0x0008
	dw 0xee00
	dw 0x0000

end:

tss:
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dq 0
	dd 0
	dw 0x10
	dw 0
	dd 0x2000

msg_timer:
	db "Timer Interrupt!", 0x0
