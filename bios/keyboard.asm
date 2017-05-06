global bsv_keyboard

BITS 16
; bsv_keyboard
bsv_keyboard:
	cmp ah, 0x02
	je keyboard_ret_sf_stat
	iret

; keyboard_ret_sf_stat
keyboard_ret_sf_stat:
	mov al, 0
	iret
