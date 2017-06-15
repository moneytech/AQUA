;  paging.asm
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

global load_page_dir
load_page_dir:
	push ebp

	mov ebp, esp
	mov eax, [esp + 8]

	mov cr3, eax
	mov esp, ebp

	pop ebp
	ret

	;push ebp
	;mov ebp, esp
	;push dword [ebp + 8]
	;pop eax

	;pusha
	;mov cr3, eax

	;mov eax, cr0
	;or eax, 0x80000000
	;mov cr0, eax
	;popa

	;mov esp, ebp
	;pop ebp
	;ret

global enable_paging
enable_paging:
	push ebp
	mov ebp, esp

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	mov esp, ebp
	pop ebp
	ret
