BITS 16
	org 0x7c00

	jmp boot
	BS_jmpBoot2	db	0x90
	BS_OEMName	db	"SCOS", 0x00, 0x00, 0x00, 0x00
	BPB_BytsPerSec	dw	0x0200		;BytesPerSector
	BPB_SecPerClus	db	0x01		;SectorPerCluster
	BPB_RsvdSecCnt	dw	0x0001		;ReservedSectors
	BPB_NumFATs	db	0x02		;TotalFATs
	BPB_RootEntCnt	dw	0x00e0		;MaxRootEntries
	BPB_TotSec16	dw	0x0b40		;TotalSectors
	BPB_Media	db	0xf0		;MediaDescriptor
	BPB_FATSz16	dw	0x0009		;SectorsPerFAT
	BPB_SecPerTrk	dw	0x0012		;SectorsPerTrack
	BPB_NumHeads	dw	0x0002		;NumHeads
	BPB_HiddSec	dd	0x00000000	;HiddenSector
	BPB_TotSec32	dd	0x00000000	;TotalSectors

	BS_DrvNum	db	0x00		;DriveNumber
	BS_Reserved1	db	0x00		;Reserved
	BS_BootSig	db	0x29		;BootSignature
	BS_VolID	dd	0x20170401	;VolumeSerialNumber
	BS_VolLab	db	"ShiftCrops "	;VolumeLabel
	BS_FilSysType	db	"FAT12   "	;FileSystemType

boot:
	mov ax, cs
	mov ds, ax

	; read sectors (0x1000:0x0)
	mov ax, 0x1000
	mov es, ax
	mov bx, 0x0

	mov ah, 0x02	; read
	mov al, 0x10	; sectors
	mov ch, 0x00	; cylinder
	mov cl, 0x02	; sector
	mov dh, 0x00	; head
	mov dl, 0x00	; drive
	;int 0x13
	nop
	nop

	; gdt
	lgdt [gdtr]

	; A20 gate
	in al, 0x92
	or al, 2
	out 0x92, al

	; Protected mode
	mov eax, cr0
	or al, 1
	mov cr0, eax

	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov sp, 0x1000
	jmp dword 0x8:0x10000

align 4
gdtr:
	dw gdt_end - gdt -1
	dd gdt
gdt:
	dq 0

	; code
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x88
	db 0x42
	db 0x00

	; data
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x82
	db 0x42
	db 0x00
gdt_end:

times	0x1fe-($-$$) db 0
	dw 0xaa55
