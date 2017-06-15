;  descriptor_tables.asm
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


	bits 32

extern gdt_ptr

global gdt_flush
gdt_flush:
	cli
	lgdt [gdt_ptr]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x08:flush2

flush2:
	ret

global tss_flush
tss_flush:
	mov ax, 0x2B
	ltr ax
	ret

global tss_install
tss_install:
	mov ax, 0x28
	ltr ax
