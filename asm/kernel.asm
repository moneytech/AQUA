;  kernel.asm
;
;  Copyright 2017 Obiwac
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation; either version 2 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program; if not, write to the Free Software
;  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
;  MA 02110-1301, USA.
;
;

; TODO: move this to src/

	bits 32

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
	align 0x1000

	global boot_page_directory
	boot_page_directory:
		dd 0x00000083
		times (KERNEL_PAGE_NUMBER - 1) dd 0

		dd 0x00000083
		times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

	retfromkernel:
		db 0xa, "|-----------------------|",0xa,"| Returned from kernel. |",0xa,"|-----------------------|"

section .text
	align 4

	dd 0x1BADB002
	dd 0x04
	dd -(0x1BADB002 + 0x04)

	dd 0
	dd 0
	dd 0
	dd 0
	dd 0

	dd 1;0
	dd 800;800
	dd 600;600
	dd 32;32

	extern main
	extern call_constructors

	global start
	start:
		cli

		mov ecx, (boot_page_directory - KERNEL_VIRTUAL_BASE)
		mov cr3, ecx

		mov ecx, cr4
		or ecx, 0x00000010
		mov cr4, ecx
		
		;mov ecx, cr0
		;or ecx, 0x80000000
		;mov cr0, ecx
		
		lea ecx, [start_hh]
		jmp ecx

	start_hh:
		mov dword [boot_page_directory], 0
		invlpg [0]

		mov esp, stack + 0x4000
		call call_constructors

		push dword ebx
		push dword eax

		call main

		mov eax, 1
		mov ebx, retfromkernel
		int 0x80
		jmp $

		call stop

	stop:
		cli

		hlt
		jmp stop

	extern user_function

	global jump_usermode
	jump_usermode:
		mov ax, 0x23
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax

		mov eax, esp
		push 0x23
		push eax

		pushf
		push 0x1B
		push user_function

		iret

	disk_load:
		push dx

		mov ah, 0x02
		mov al, dh
		mov ch, 0x00
		mov dh, 0x00

	disk_load_go:
		int 0x13
		jc disk_error
		pop dx
		cmp dh, al

		jne disk_error
		ret

	disk_error:
		jmp $

section .bss
	align 32

	stack:
		resb 0x4000
