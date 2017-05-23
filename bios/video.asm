global bsv_video, cursor_x, cursor_y

BITS 16
; bsv_video
bsv_video:
	cmp ah, 0x00
	je video_set_video_mode
	cmp ah, 0x02
	je video_set_cursor_pos
	cmp ah, 0x03
	je video_get_cursor_pos
	cmp ah, 0x06
	je video_scroll_up
	cmp ah, 0x0e
	je video_write_teletype
	cmp ah, 0x13
	je video_write_string
	iret

; video_set_video_mode
video_set_video_mode:
	mov dx, 0x3c4
	mov al, 2
	out dx, al
	mov dx, 0x3c5
	mov al, 0x4
	out dx, al
	mov dx, 0x3c4
	mov al, 4
	out dx, al
	mov dx, 0x3c5
	mov al, 0x6
	out dx, al

	mov dx, 0x3ce
	mov al, 5
	out dx, al
	mov dx, 0x3cf
	mov al, 0x0
	out dx, al
	mov dx, 0x3ce
	mov al, 6
	out dx, al
	mov dx, 0x3cf
	mov al, 0x5
	out dx, al
	iret

; video_set_cursor_pos
video_set_cursor_pos:
	mov [cursor_x], dl
	mov [cursor_y], dh
	mov ax, 0x0
	iret

; video_get_cursor_pos
video_get_cursor_pos:
	mov dl, [cursor_x]
	mov dh, [cursor_y]
	mov ax, 0x0
	iret

; video_scroll_up
video_scroll_up:
	pusha
	push ds
	mov cx, 0xb800
	mov ds, cx
	xor ah, ah
	sub word [cursor_y], ax
	mov si, ax
	imul si, 0x28*2
	xor di, di
	mov cx, 0x19	; 0xc8/0x8
	sub cx, ax
	imul cx, 0x28
video_up_loop:
	mov dx, [si]
	mov [di], dx
	add si, 2
	add di, 2
	dec cx
	test cx, cx
	jnz video_up_loop

	xor bl, bl
video_up_attr_loop:
	mov [di], bx
	add di, 2
	cmp di, 0x28*2*0x19
	jl video_up_attr_loop
	pop ds
	popa
	iret

; video_write_teletype
video_write_teletype:
	pusha
	push ds
	mov cx, 0xb800
	mov ds, cx
	mov di, [cursor_y]
	imul di, 0x28
	add di, [cursor_x]
	imul di, 2
	mov [di], al
	inc di
	mov [di], bl

	mov di, [cursor_x]
	inc di
	cmp di, 0x28
	jl video_teletype_end

	mov di, 0
	inc word [cursor_y]
video_teletype_end:
	mov [cursor_x], di
	pop ds
	popa
	iret

; video_write_string
video_write_string:
	pusha
	push ds
	push dx
	mov cx, 0xb800
	mov ds, cx

	xor cx, cx
	mov cl, dh
	imul cx, 0x28
	xor dh, dh
	add dx, cx
	imul di, dx, 2

	xor cx, cx
video_string_loop:
	mov ah, [es:bp]
	cmp ah ,0x0
	jz video_string_end
	mov [di], ah
	inc di
	cmp al, 0x2
	jl video_string_skip_attr
	mov [di], bl
video_string_skip_attr:
	inc bp
	inc di
	inc cx
	jmp video_string_loop

video_string_end:
	pop dx
	test al, 1
	jz video_string_skip_cursor
	mov [cursor_x], dl
	add [cursor_x], cx
	mov [cursor_y], dh
video_string_skip_cursor:
	pop ds
	popa
	iret

cursor_x:
	dw 0
cursor_y:
	dw 0
